#include "Core.h"
#include "Map.h"
#include "Configuration.h"
#include "MapFactory.h"
#include "Path.h"
#include "GuidingPathFactory.h"
#include "MapProcessor.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#define PI 3.14159265358979323846
#include <cmath>
#include <memory>
#include <string>
#include "Output.h"
#include "State.h"
#include "Random.h"
#include "UavGroup.h"

using namespace std;

namespace App
{

	Core::Core(shared_ptr<Configuration> configuration) :
		logger(make_shared<LoggerInterface>()), configuration(configuration)
	{
		setLogger(make_shared<LoggerInterface>());	//I will use LoggerInterface as NilObject for Logger, because I am too lazy to write NilObject Class.

		MapFactory mapFactory;
		maps = mapFactory.createMaps(configuration->getUavCount());
	}


	Core::~Core()
	{
	}

	void Core::run()
	{
		clock_t start;
		double duration;

		start = clock();

		shared_ptr<Map> map = maps.at(configuration->getMapNumber());
		logger->logSelectedMap(map, configuration->getWorldWidth(), configuration->getWorldHeight());
		MapProcessor mapProcessor = MapProcessor(logger);
		auto nodes = mapProcessor.mapToNodes(map, configuration->getAStarCellSize(), configuration->getWorldWidth(), configuration->getWorldHeight(), configuration->getUavSize());
		GuidingPathFactory pathFactory = GuidingPathFactory(logger);
		auto paths = pathFactory.createGuidingPaths(nodes->getAllNodes(), nodes->getStartNode(), nodes->getEndNodes());

		duration = (clock() - start) / double(CLOCKS_PER_SEC);

		cout << to_string(duration) << "seconds to discretize map and find path" << endl;

		rrtPath(paths, configuration, map);
	}

	void Core::setLogger(shared_ptr<LoggerInterface> logger)
	{
		this->logger = logger;
	}

	void Core::logConfigurationChange()
	{
		auto map = maps.at(configuration->getMapNumber());
		logger->logSelectedMap(map, configuration->getWorldWidth(), configuration->getWorldHeight());
	}

	void Core::rrtPath(vector<shared_ptr<Path>> guiding_paths, shared_ptr<Configuration> configuration, shared_ptr<Map> map)
	{
		int uavCount = configuration->getUavCount();
		int rrt_min_nodes = 1;
		int rrt_max_nodes = 20000;
		bool stop = false;
		int number_of_solutions = 10000;
		int near_count = 1000;
		int number_of_inputs = 12;	//todo: spoèítat z dalších hodnot z konfigurace, je to v Node.h
		bool debug = true;
		int distance_of_new_nodes = 30;
		int guiding_near_dist = 40;

		cout << "Starting RRT-path...";

		//todo: implementovat init_goals
		// celková dráha všech cest dohromady, poèet nodes ve všech cestách
		// dohromady, evidentnì se ta promìnná nikde nepoužívá
		int gp_length = 0;
		for (int m = 0; m < guiding_paths.size(); m++)
		{
			gp_length += guiding_paths[m]->getSize();
		}

		vector<shared_ptr<State>> nodes = vector<shared_ptr<State>>(); //todo: zjistit, na jaké hodnoty to inicializovat
		auto initialState = make_shared<State>();
		initialState->uavs = map->getUavsStart();
		nodes.push_back(initialState);


		auto current_index = vector<vector<int>>(uavCount);		// matice s délkami cest pro jednotlivá UAV.sloupec je cesta, øádek je UAV

		for (int i = 0; i < current_index.size(); i++)
		{
			current_index[i] = vector<int>(guiding_paths.size());
			for (int j = 0; j < guiding_paths.size(); j++)
			{
				current_index[i][j] = guiding_paths[j]->getSize() - 1;	//protože se indexuje od 0
			}
		}

		auto final_nodes = vector<shared_ptr<State>>(rrt_max_nodes);

		auto goals_reached = vector<bool>(uavCount);
		for (auto goal_reached : goals_reached)
		{
			goal_reached = false;
		}


		int nodes_count = rrt_max_nodes;
		vector<int> goal_reached_by_all_uavs;
		shared_ptr<State> new_node;
		shared_ptr<State> near_node;
		auto output = make_shared<Output>();

		int i = 1; // poèet expandovaných nodes
		int m = 1; // poèet nalezených cest
		int s = 2; // poèet prùchodù cyklem ? prostì se to jen zvìtší o 1 pøi každém prùchodu, nikde se nepoužívá

		while ((m <= number_of_solutions || i < rrt_min_nodes) && i < rrt_max_nodes) // numver_of_solutions je asi 10 000.
		{
			if (stop)
			{
				break;
			}
			i++;

//			%Random state
			vector<shared_ptr<Point>> s_rand = random_state_guided(guiding_paths, current_index, goals_reached, map); // vrátí pole náhodných bodù, jeden pro každou kvadrokoptéru

			//Finding appropriate nearest neighbor
			int k = 0;	//poèítadlo uvíznutí
			bool near_found = false;

			auto isNewUavPosition = false;
			//opakování, dokud nenajdu vyhovující øešení, poèítají se prùchody cyklem kvùli uvíznutí
			while (!near_found)
			{
				if (k > near_count)
				{
					i--;
					throw "Not possible to find near node suitable for expansion";
				}
				near_node = nearest_neighbor(s_rand, nodes, k);
				vector<shared_ptr<State>> returnedNodes = select_input(s_rand, near_node);
				// Vypadá to, že near_node je ve funkci select_input zmìnìná kvùli kontrole pøekážek
				near_node = returnedNodes[0];
				new_node = returnedNodes[1];
				nodes.push_back(near_node); // promìnná nodes je pole, kam se ukládá strom prohledávání u RRT - Path. Nemìlo by být potøeba tohle pøiøazovat, protože tam je reference, ne hodnota

				bool allInputsUsed = true;
				for (bool inputUsed : near_node->used_inputs)
				{
					allInputsUsed = allInputsUsed && inputUsed;
				}

				auto isNearUavPosition = false;
				for (auto uavPosition : near_node->uavs)
				{
					isNearUavPosition = isNearUavPosition || (uavPosition.get() != nullptr);	//pozice je null, pokud se pro UAV nenašla vhodná další pozice
				}

				isNewUavPosition = false;
				for (auto uavPosition : new_node->uavs)
				{
					isNewUavPosition = isNewUavPosition || (uavPosition.get() != nullptr);	//pozice je null, pokud se pro UAV nenašla vhodná další pozice
				}

				//poèítadlo uvíznutí. UAV uvízlo, pokud je tento if true
				if (allInputsUsed || !isNewUavPosition || !isNearUavPosition)
				{
					k++;
					check_expandability(nodes);
				} else
				{
					near_found = true;
				}
			}

			if (!isNewUavPosition)
			{
				check_expandability(nodes);
				cout << "NaN in new node";
				final_nodes[m] = nodes[i - 1];
				break;
			}

			new_node->index = i;
			if (debug)
			{
				cout << "[debug] Added node index: " << new_node->index << endl;
			}

			if (i % 200 == 0)
			{
				printf("RRT size: %d\n", i);
			}

			nodes[i] = new_node;
			s++;

			current_index = guiding_point_reached(new_node, guiding_paths, current_index, guiding_near_dist); // zde se uloží do current_index, kolik nodes zbývá danému UAV do cíle
			goal_reached_by_all_uavs = check_near_goal(new_node->uavs, map);

			bool isGoalReached = true;
			for (int goal_reached : goal_reached_by_all_uavs)
			{
				isGoalReached = isGoalReached && (goal_reached > 0);
			}

			output->distance_of_new_nodes = vector<int>(nodes.size());
			if (isGoalReached) // pokud je nalezen cíl
			{
				output->goal_reached = goal_reached_by_all_uavs;
				final_nodes[m] = new_node;
				printf("%d viable paths found so far.\n", m);
				m++;
			}
			output->distance_of_new_nodes[i] = distance_of_new_nodes;
		//todo: udìlat visualizaci rùstu
			if (i % configuration->getDrawPeriod() == 0)
			{
				logger->logNewState(near_node, new_node);
				logger->logRandomStates(s_rand);
			}
		}
		
		final_nodes[m] = nodes[i];
		goal_reached_by_all_uavs = check_near_goal(new_node->uavs, map);
		output->goal_reached = goal_reached_by_all_uavs;
		//todo: ošetøit nodes a final_nodes proti nullpointerùm a vyházet null nody
		output->nodes = nodes;
		cout << "RRT-Path finished";
	}

	vector<shared_ptr<Point>> Core::random_state_guided(vector<shared_ptr<Path>> guiding_paths, vector<vector<int>> current_index, vector<bool> goals_reached, shared_ptr<Map> map)
	{
		double guided_sampling_prob = 1;
		int worldWidth = 1000;
		int worldHeight = 1000;
		int number_of_uavs = map->getUavsStart().size();
		vector<shared_ptr<Point>> randomStates = vector<shared_ptr<Point>>();

//		return vector<shared_ptr<Point>>();
		vector<double> propabilities = vector<double>(guiding_paths.size());
		
//		global number_of_uavs params

		int sum = 0; // sum = celková délka všech vedoucích cest

		// delší cesta má vìtší prpst.proto, aby algoritmus asymptiticky pokryl každou cestu stejnì hustì. na delší cestu tedy pøipadne více bodù.
		for (size_t i = 0; i < guiding_paths.size(); i++)
		{
			int pathSize = guiding_paths[i]->getSize(); // èím delší cesta, tím vìtší pravdìpodobnost, že se tou cestou vydá, aspoò doufám
			propabilities[i] = pathSize;		// propabilities je normální, 1D pole. nechápu, proè tady používá notaci pro 2D pole
			sum += pathSize;
		}
		
		for (size_t i = 0; i < guiding_paths.size(); i++)
		{
			propabilities[i] /= sum;
		}

		vector<shared_ptr<Point>> s_rand = vector<shared_ptr<Point>>(number_of_uavs);
		
		double random = Random::fromZeroToOne();
		if (random > guided_sampling_prob) //vybírá se náhodný vzorek
		{
			for (size_t i = 0; i < number_of_uavs; i++)
			{
				if (goals_reached[i])
				{//todo: s tímhle nìco udìlat, a nepøistupovat k poli takhle teple pøes indexy
					s_rand[i] = random_state_goal(map->getGoals()[goals_reached[i]], map);	//pokud je n-té UAV v cíli, vybere se náhodný bod z cílové plochy, kam UAV dorazilo
				} else
				{
					s_rand[i] = random_state(0, worldWidth, 0, worldHeight, map); // pokud n-té UAV není v cíli, vybere se náhodný bod z celé mapy
				}
			}
		} else
		{
			//rozdìlit kvadrokoptéry na skupinky. Jedna skupina pro každé AoI. Rozdìlit skupiny podle plochy, kterou jednotklivá AoI zabírají
			auto ratios = vector<double>(map->getGoals().size()); //pomìry jednotlivých ploch ku celkové ploše. Dlouhé jako poèet cílù, tedy poèet guiding paths
			double totalVolume = 0;
			for (size_t i = 0; i < map->getGoals().size(); i++)
			{
				double volume = map->getGoals()[i]->rectangle->getVolume();
				ratios[i] = volume;
				totalVolume += volume;
			}
			for_each(ratios.begin(), ratios.end(), [totalVolume](double ratio) { return ratio /= totalVolume; });	//každý prvek je v rozsahu od 0 do 1

			vector<shared_ptr<UavGroup>> uavGroups = vector<shared_ptr<UavGroup>>(map->getGoals().size());

			double uavsPerUnit = map->getUavsStart().size() / totalVolume;	// poèet UAV na jednotku celkové plochy. Po vynásobením plochou dané AoI získám poèet UAV na danou AoI.

			//pøerozdìlování kvadrokoptér podle pomìru inspirováno tímto https://github.com/sebastianbergmann/money/blob/master/src/Money.php#L261

			int uavsInGroups = 0;	//poèítá, kolik uav je rozvržených do skupin, kvùli zaokrouhlování
			for (size_t i = 0; i < uavGroups.size(); i++)
			{
				int uavsCountInGroup = floor(uavsPerUnit * ratios[i]);	//round down
				auto uavs = vector<shared_ptr<PointParticle>>(uavsCountInGroup);
				vector<int> indexes = vector<int>(uavsCountInGroup);
				for (size_t j = 0; j < uavsCountInGroup; j++)
				{
					uavs[j] = map->getUavsStart()[uavsInGroups + j];	//todo: tuhle èást asi zrefaktorovat. A nìkde mít objekty reprezentující uav, s jeho polohou, apod.
					indexes[j] = uavsInGroups + j;
				}
				uavGroups[i] = make_shared<UavGroup>(uavs, guiding_paths[i], indexes);
				uavsInGroups += uavsCountInGroup;
			}
			//rozházet do skupin nepøiøazená uav, která zbyla kvùli zaokrouhlování dolù
			int remaining = map->getUavsStart().size() - uavsInGroups;
			for (size_t i = 0; i < remaining; i++)	//zbytku je vždycky stejnì nebo ménì než poètu skupin
			{
				uavGroups[i]->addUav(map->getUavsStart()[uavsInGroups + i], uavsInGroups + i);
			}

			for (size_t i = 0; i < uavGroups.size(); i++)
			{
				auto group = uavGroups[i];
				for (size_t j = 0; j < group->getUavs().size(); j++)
				{
					int index = group->getUavIndexes()[j];
					if (goals_reached[index])	//todo: zjistit, jestli tam nemá být index cíle
					{
						randomStates.push_back(random_state_goal(map->getGoals()[goals_reached[index]], map));
					}
					else
					{
						int currentPathIndex = current_index[index][i];
						auto center = group->getGuidingPath()->get(currentPathIndex);
						randomStates.push_back(random_state_polar(center->getPoint(), map, 0, configuration->getSamplingRadius()));
					}
				}
			}
		}
		return randomStates;
	}

	shared_ptr<State> Core::nearest_neighbor(vector<shared_ptr<Point>> s_rand, vector<shared_ptr<State>> nodes, int count)
	{
		int max_nodes = 20000;
		int debug = true;
		int nn_method = 1;

		vector<shared_ptr<State>> near_arr = vector<shared_ptr<State>>();
		shared_ptr<State> near_node = nodes[0];

		int s = 1;
		int current_best = INT32_MAX;
		
		for (int j = 0; j < max_nodes; j++)
		{
			// Distance of next node in the tree
			bool isNull = j >= nodes.size();
			if (isNull)
			{
				if (debug)
				{
					printf("NaN in node %d\n", j);
				}
				break;
			}
			shared_ptr<State> tmp_node = nodes[j];	//todo: refactorovat, aby se nesahalo do prázdných nodes

			if (tmp_node->areAllInputsUsed())
			{
				printf("Node %d is unexpandable\n", tmp_node->index);
				continue;
			}
			
			double hamilt_dist = 0;
			vector<double> distances = vector<double>(tmp_node->uavs.size());

			for (size_t i = 0; i < tmp_node->uavs.size(); i++)
			{
				auto uav = tmp_node->uavs[i];
				auto randomState = s_rand[i];	//todo: refactoring: udìlat metodu na vzdálenosti bodù do nìjakého bodu, a to nemám všude rozprcané
				distances[i] = pow(uav->getLocation()->getX() - randomState->getX(), 2) + pow(uav->getLocation()->getY() - randomState->getY(), 2);
			}

			switch (nn_method)
			{
			case 1:
				for(auto dist : distances)
				{
					hamilt_dist += dist;	//no function for sum
				}
				break;
			case 2:
				hamilt_dist = *std::max_element(distances.begin(), distances.end());	//tohle vrací iterátor, který musím dereferencovat, abych získal èíslo. fuck you, C++
				break;
			case 3:
				hamilt_dist = *std::min_element(distances.begin(), distances.end());	//tohle vrací iterátor, který musím dereferencovat, abych získal èíslo. fuck you, C++
				break;
			}

			
			
			//Check if tested node is nearer than the current nearest
			if (hamilt_dist < current_best)
			{
				near_arr.push_back(near_node);
				current_best = hamilt_dist;
				near_node = nodes[j];
				if (debug)
				{
					double distance;
					for (size_t i = 0; i < tmp_node->uavs.size(); i++)
					{
						auto uav = tmp_node->uavs[i];
						auto randomState = s_rand[i];	//todo: refactoring: udìlat metodu na vzdálenosti bodù do nìjakého bodu, a to nemám všude rozprcané
						distance = pow(uav->getLocation()->getX() - randomState->getX(), 2) + pow(uav->getLocation()->getY() - randomState->getY(), 2);
					}
					printf("[debug] near node #%d found, distance to goal state: %f\n", s, distance);
				}
				s++;
			}			
		}
			
		if (near_arr.size() > count)
		{
			near_node = near_arr[near_arr.size() - count];	//todo: zkontrolovat, jestli sedí pøesnì index a jestli to neubíhá o 1
			double distance;
			for (size_t i = 0; i < near_node->uavs.size(); i++)
			{
				auto uav = near_node->uavs[i];
				auto randomState = s_rand[i];	//todo: refactoring: udìlat metodu na vzdálenosti bodù do nìjakého bodu, a to nemám všude rozprcané
				distance = pow(uav->getLocation()->getX() - randomState->getX(), 2) + pow(uav->getLocation()->getY() - randomState->getY(), 2);
			}
			if (debug && count > 0)
			{
				printf("[debug] near node #%d chosen, %d discarded, near node index %d, distance to goal state: %f\n", near_arr.size() - count, count, near_node->index, distance);
			}
		}

		return near_node;
	}

	vector<shared_ptr<State>> Core::select_input(vector<shared_ptr<Point>> s_rand, shared_ptr<State> near_node)
	{
		int input_samples_dist = 1;
		int input_samples_phi = 3;
		int distance_of_new_nodes = 30;
		double max_turn = PI / 200;
		bool relative_localization = true;
		int uavCount = near_node->uavs.size();
		int inputCountPerUAV = input_samples_dist * input_samples_phi;
		int inputCount = pow(inputCountPerUAV, uavCount);
		vector<shared_ptr<Point>> oneUavInputs = vector<shared_ptr<Point>>();
		shared_ptr<State> new_node;

		for (size_t k = 0; k < input_samples_dist; k++)
		{
			for (size_t m = 0; m < input_samples_phi; m++)
			{
				double x = distance_of_new_nodes / pow(1.5,k);
				double y = max_turn + 2 * m * max_turn / (input_samples_phi - 1);
				shared_ptr<Point> point = make_shared<Point>(x, y);
				oneUavInputs.push_back(point);
			}
		}

		//poèet všech možných "kombinací" je variace s opakováním (n-tuple anglicky). 
		//inputs jsou vstupy do modelu
		vector<vector<shared_ptr<Point>>> inputs = generateNTuplet<shared_ptr<Point>>(oneUavInputs, inputCountPerUAV);	//poèet všech kombinací je poèet všech možných vstupù jednoho UAV ^ poèet UAV
		//translations jsou výstupy z modelu
		vector<vector<shared_ptr<Point>>> translations = vector<vector<shared_ptr<Point>>>(inputCount);	//poèet všech kombinací je poèet všech možných vstupù jednoho UAV ^ poèet UAV
		vector<shared_ptr<State>> tempStates = vector<shared_ptr<State>>(inputCount);	//poèet všech kombinací je poèet všech možných vstupù jednoho UAV ^ poèet UAV

		for (size_t i = 0; i < inputs.size(); i++)
		{
			auto input = inputs[i];
			auto tempState = car_like_motion_model(near_node, input);
			tempStates[i] = tempState;
			translations[i] = vector<shared_ptr<Point>>();
			for (size_t j = 0; j < tempState->uavs.size(); j++)
			{
				double x = tempState->uavs[j]->getLocation()->getX() - near_node->uavs[j]->getLocation()->getX();
				double y = tempState->uavs[j]->getLocation()->getY() - near_node->uavs[j]->getLocation()->getX();
				translations[i][j] = make_shared<Point>(x ,y);
			}
		}

		vector<double> d = vector<double>(inputCount);
		//todo: možná zrefactorovat a schovat do jednoho cyklu, který je výše
		//todo: kde to pùjde, pužít range-based loop, iteraci místo klasického foru
		//Distance to s_rand when using different inputs
		for (size_t i = 0; i < d.size(); i++)
		{
			auto tempState = tempStates[i];
			d[i] = 0;
			for (size_t j = 0; j < tempState->uavs.size(); j++)
			{
				double x = s_rand[j]->getX() - tempState->uavs[j]->getLocation()->getX();
				double y = s_rand[j]->getY() - tempState->uavs[j]->getLocation()->getY();
				d[i] += sqrt(x*x + y*y);
			}
		}

		
		// Find vector with minimal distance to s_rand and return it
		if (relative_localization)
		{
			int m = 0;
			while (m < inputCount)
			{
				m++;
				if (near_node->areAllInputsUsed())
				{
					cout << "No valid input left";
					break;
				}
				int index = 0;	//klíè nejmenší hodnoty vzdálenosti v poli d
				double minValue = DBL_MAX;
				for (size_t i = 0; i < d.size(); i++)
				{
					if (d[i] < minValue)
					{
						index = i;
					}
				}
				auto tempState = tempStates[index];

				if (!check_localization_sep(tempState) || trajectory_intersection(near_node, tempState) || near_node->used_inputs[index])
				{
					d[index] = NULL;
					continue;
				}

				if (!check_world_bounds(tempState->uavs, configuration->getWorldWidth(), configuration->getWorldHeight()))
				{
					d[index] = NULL;
					continue;
				}

				near_node = check_obstacle_vcollide_single(near_node, translations, index);

				if (near_node->used_inputs[index])
				{
					d[index] = NULL;
					continue;
				} else
				{
					new_node = make_shared<State>();
					new_node->uavs = tempState->uavs;
					new_node->prev = near_node;
					new_node->prev_inputs = tempState->prev_inputs;
					near_node->used_inputs[index] = true;
					break;
				}
			}
		} else
		{
			//todo: rozhodnout, zda to tady chci, nebo zda bude natvrdo zapnutá relativní lokalizace
			//vùbec nechápu, co tady dìlá m. Je úplnì out odf scope, zùstává na poslední hodnotì for cyklu.
//		    if ~near_node.used_inputs(m,1)
//		        index = find(d(:) == min(d(:)),1);
//		        tmp_node.loc = tmp_nodes(index(1,n)).loc;
//		        tmp_node.rot = tmp_nodes(index(1,n)).rot;
//		        new_node = tmp_node;
//		        new_node.prev = near_node.index;
//		        new_node.prev_inputs(:,:) = tmp_nodes(index(1,n).prev_inputs(:,:));
//		        near_node.used_inputs(index,1) = true;
//		    end
		}		
		
		return {near_node, new_node};
	}

	int Core::check_expandability(vector<shared_ptr<State>> nodes)
	{
		int unexpandable_count = 0;
		for (auto node : nodes)
		{
			if (node->areAllInputsUsed())
			{
				unexpandable_count++;
			}
		}

		if (unexpandable_count > 0) {
			printf("Not expandable nodes: %d/%d\n", unexpandable_count, nodes.size());
		}
		return unexpandable_count;
	}

	//todo: popøemýšlet na refactoringem current_index, abych tady nepracoval s maticí
	//detects narrow passage
	vector<vector<int>> Core::guiding_point_reached(shared_ptr<State> node, vector<shared_ptr<Path>> guiding_paths, vector<vector<int>> current_index, int guiding_near_dist)
	{
		for (int k = 0; k < guiding_paths.size(); k++) {
			auto guiding_path = guiding_paths[k];
			for (int m = 0; m < guiding_path->getSize(); m++) {
				for (int n = 0; n < current_index.size(); n++) {
					bool reached = false;
					if ((pow(node->uavs[n]->getLocation()->getX() - guiding_path->get(m)->getPoint()->getX(), 2) + pow(node->uavs[n]->getLocation()->getY() - guiding_path->get(m)->getPoint()->getY(), 2)) < pow(guiding_near_dist, 2))
					{
						reached = true;
						//Narrow passage detection
						detect_narrow_passage(guiding_path->get(m));
					}
					if (reached == true && m > 0 && m <= current_index[n][k]) 
					{
						current_index[n][k] = m - 1;	//todo: zjistit, jestlimám opravdu odeèítat jednièku
						break;
					}
				}
			}
		}
		return current_index;
	}

	vector<int> Core::check_near_goal(vector<shared_ptr<PointParticle>> uavs, shared_ptr<Map> map)
	{
		auto goal_reached = vector<int>(uavs.size());//todo: místo pole intù pøedìlat na pole goal objektù a místo nuly bude null pointer nebo tak nìco

		for (int m = 0; m < map->getGoals().size(); m++)
		{
			for (int n = 0; n < uavs.size(); n++)
			{
				if (map->getGoals()[m]->is_near(uavs[n]->getLocation()))
				{
					goal_reached[n] = m;
				} else
				{
					goal_reached[n] = 0;
				}
			}
		}
		return goal_reached;
	}

	void Core::detect_narrow_passage(shared_ptr<Node> node)
	{
		//todo: vymyslet, jak to udìlat, abych odsud nastavil parametry, ale nesahal na celou konfiguraci. udìlat asi nìjaký command na dìlení np_divisorem

//		global params defaults count
		if (node->getCost() > 1) {
//			params.distance_of_new_nodes = defaults.distance_of_new_nodes / params.np_divisor;
//			params.max_turn = defaults.max_turn * params.np_divisor;
//			params.guiding_near_dist = defaults.guiding_near_dist / params.np_divisor;
//			count = 0;
//			// params.sampling_radius = defaults.sampling_radius / 3;
		} else {
//			count = count + 1;
//		
//			//Far from obstacle
//				if count > params.exit_np_threshold
//					params.distance_of_new_nodes = defaults.distance_of_new_nodes;
//					params.max_turn = defaults.max_turn;
//					params.guiding_near_dist = defaults.guiding_near_dist;
//				count = 0;
//				end
//		// params.sampling_radius = defaults.sampling_radius;
		}

	}

	shared_ptr<Point> Core::random_state_goal(shared_ptr<Goal> goal, shared_ptr<Map> map)
	{
		return random_state(goal->rectangle, map);
	}

	shared_ptr<Point> Core::random_state(shared_ptr<Rectangle> rectangle, shared_ptr<Map> map)
	{
		return random_state(rectangle->getX(), rectangle->getX() + rectangle->getWidth(), rectangle->getY(), rectangle->getY() + rectangle->getHeight(), map);
	}

	shared_ptr<Point> Core::random_state(int x1, int y1, int x2, int y2, shared_ptr<Map> map)
	{
		shared_ptr<Point> random_state;
		do
		{
			int x = Random::inRange(x1, x2);
			int y = Random::inRange(y1, y2);
			random_state = make_shared<Point>(x, y);
		} while (check_inside_obstacle(random_state, map));
		return random_state;
	}

	//returns true if point is inside of obstacle
	bool Core::check_inside_obstacle(shared_ptr<Point> point, shared_ptr<Map> map)
	{
		bool collision = false;

		for (auto obstacle : map->getObstacles())
		{
			collision = collision || obstacle->rectangle->contains(point);
		}
		return collision;
	}

	shared_ptr<Point> Core::random_state_polar(shared_ptr<Point> center, shared_ptr<Map> map, double radius_min, double radius_max)
	{
		shared_ptr<Point> randomState;
		do
		{
			double phi = Random::inRange(0, 2 * PI);
			double r = radius_min + Random::inRange(radius_min, radius_max);
			double x = center->getX() + r*cos(phi);
			double y = center->getY() + r*sin(phi);
			randomState = make_shared<Point>(x, y);
		} while (check_inside_obstacle(randomState, map) && check_world_bounds(randomState, configuration->getWorldWidth(), configuration->getWorldHeight()));
		return randomState;
	}

	bool Core::check_world_bounds(shared_ptr<Point> point, int worldWidth, int worldHeight)
	{
		bool inBounds = false;

		if (point->getX() < worldWidth && point->getX() > 0 && point->getY() < worldHeight && point->getY() > 0)
		{
			inBounds = true;
		}
		return inBounds;

	}

	bool Core::check_world_bounds(vector<shared_ptr<PointParticle>> points, int worldWidth, int worldHeight)
	{
		bool inBounds = false;
		for (auto point : points)
		{
			inBounds = inBounds || check_world_bounds(point->getLocation(), worldWidth, worldHeight);
		}
		return inBounds;
	}

	shared_ptr<State> Core::car_like_motion_model(shared_ptr<State> node, vector<shared_ptr<Point>> inputs)
	{
		//todo: implementovat
		return shared_ptr<State>();
	}

	bool Core::check_localization_sep(shared_ptr<State> node)
	{
		//todo: implementovat
		return false;
	}

	bool Core::trajectory_intersection(shared_ptr<State> near_node, shared_ptr<State> tmp_node)
	{
		//todo: implementovat
		return false;
	}

	shared_ptr<State> Core::check_obstacle_vcollide_single(shared_ptr<State> near_node, vector<vector<shared_ptr<Point>>> translation, int index)
	{
		//todo: implementovat
		return shared_ptr<State>();
	}

	// Zde je java implementace, ze které vycházím https ://cs.wikipedia.org/wiki/Variace_(algoritmus)
	template<typename T> vector<vector<T>> Core::generateNTuplet(vector<T> usedChars, int tupletClass)
	{
		vector<vector<T>> list = vector<vector<T>>();	//todo: popøemýšlet, jak to refactorovat, aby se mohlo pracovat s fixní velikostí  pole
//		vector<vector<T>> list = vector<vector<T>>(pow(usedChars.size(), tupletClass));

		if(tupletClass == 0)
		{
			list.push_back(vector<T>());
		} else
		{
			vector<vector<T>> tuplet = generateNTuplet(usedChars, tupletClass - 1);
			for (T character : usedChars)
			{
				for (auto row : tuplet)
				{
					list.push_back(row.push_back(character));
				}
			}
		}
		return list;
	}

}
