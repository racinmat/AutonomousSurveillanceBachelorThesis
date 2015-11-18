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
#include <cmath>
#include <memory>
#include <string>
#include "Output.h"
#include "State.h"
#include "Random.h"
#include "UavGroup.h"
#include "VCollide/Triangle3D.h"
#include "VCollide/ColDetect.h"
#include <chrono>
#include <thread>
#include "Enums.h"

#define PI 3.14159265358979323846

using namespace std;

namespace App
{

	Core::Core(shared_ptr<Configuration> configuration) :
		logger(make_shared<LoggerInterface>()), configuration(configuration), 
		stateFactory(make_shared<StateFactory>(configuration))
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
		file = ofstream("myLogging.txt");
		
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


		//protoe Petrlík má guidingPath otoèenou, také si ji otoèím, abych to mìl pro kontrolu stejnì
		//todo: potom vše zrefactorovat tak, aby mohla bıt cesta správnì, neotoèená
		for (auto path : paths)
		{
			path->reverse();
		}

//		try
//		{
			rrtPath(paths, configuration, map);
			file.close();
//		} catch(const rrtPathError& error)
//		{
			file.close();
//			throw error;
//		}
//		testGui();

	}

	void Core::testGui()
	{
		for (size_t i = 0; i < 200; i++)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			this->logger->logText(to_string(i));
		}
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
//		throw 22;
//		throw runtime_error("No valid input found.");

		int uavCount = configuration->getUavCount();
		int rrt_min_nodes = configuration->getRrtMinNodes();
		int rrt_max_nodes = configuration->getRrtMaxNodes();
		bool stop = false;	//todo: udìlat naèítání stop z konfigurace. konfiguraci mìnit z gui.
		int number_of_solutions = configuration->getNumberOfSolutions();
		int near_count = configuration->getNearCount();
		bool debug = configuration->getDebug();
		int distance_of_new_nodes = configuration->getDistanceOfNewNodes();
		int guiding_near_dist = configuration->getGuidingNearDist();

		cout << "Starting RRT-path...";

		//todo: implementovat init_goals
		// celková dráha všech cest dohromady, poèet nodes ve všech cestách
		// dohromady, evidentnì se ta promìnná nikde nepouívá
		int gp_length = 0;
		for (int m = 0; m < guiding_paths.size(); m++)
		{
			gp_length += guiding_paths[m]->getSize();
		}

		vector<shared_ptr<State>> nodes = vector<shared_ptr<State>>(); //todo: zjistit, na jaké hodnoty to inicializovat
		auto initialState = stateFactory->createState();
		initialState->uavs = map->getUavsStart();
		nodes.push_back(initialState);


		auto current_index = vector<vector<int>>(uavCount);		// matice s délkami cest pro jednotlivá UAV.sloupec je cesta, øádek je UAV

		for (int ii = 0; ii < current_index.size(); ii++)	//promìnná ii, aby se pøi debuggingu nepletla s promìnnou i, která se pouívá ve velkém while cyklu
		{
			current_index[ii] = vector<int>(guiding_paths.size());
			for (int j = 0; j < guiding_paths.size(); j++)
			{
				current_index[ii][j] = guiding_paths[j]->getSize() - 1;	//protoe se indexuje od 0
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

		int i = 0; // poèet expandovanıch nodes, hned na zaèátku se zvıší o jedna
		int m = 1; // poèet nalezenıch cest
		int s = 2; // poèet prùchodù cyklem ? prostì se to jen zvìtší o 1 pøi kadém prùchodu, nikde se nepouívá

		while ((m <= number_of_solutions || i < rrt_min_nodes) && i < rrt_max_nodes) // number_of_solutions je asi 10 000.
		{
			if (stop)
			{
				break;
			}
			i++;

//			%Random state
			vector<shared_ptr<Point>> s_rand = random_state_guided(guiding_paths, current_index, goals_reached, map); // vrátí pole náhodnıch bodù, jeden pro kadou kvadrokoptéru

			//Finding appropriate nearest neighbor
			int k = 1;	//poèítadlo uvíznutí, protoe se pouívá v nearest_neighbor size() - k, zaèínám od 1
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
				vector<shared_ptr<State>> returnedNodes = select_input(s_rand, near_node, map);	
				// Vypadá to, e near_node je ve funkci select_input zmìnìná kvùli kontrole pøekáek
				near_node = returnedNodes[0];
				new_node = returnedNodes[1];
				nodes.push_back(near_node); // promìnná nodes je pole, kam se ukládá strom prohledávání u RRT - Path. Nemìlo by bıt potøeba tohle pøiøazovat, protoe tam je reference, ne hodnota

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

			if (i < nodes.size())
			{
				nodes[i] = new_node;
			} else
			{
				nodes.push_back(new_node);
			}
			s++;

			current_index = guiding_point_reached(new_node, guiding_paths, current_index, guiding_near_dist); // zde se uloí do current_index, kolik nodes zbıvá danému UAV do cíle
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
		double guided_sampling_prob = configuration->getGuidedSamplingPropability();
		int worldWidth = configuration->getWorldWidth();
		int worldHeight = configuration->getWorldHeight();
		int number_of_uavs = map->getUavsStart().size();
		vector<shared_ptr<Point>> randomStates = vector<shared_ptr<Point>>();

//		return vector<shared_ptr<Point>>();
		vector<double> propabilities = vector<double>(guiding_paths.size());	//tohle nakonec vùbec není pouito, protoe se cesty urèily pøesnì. 
		//todo: vyøešit problém s tím, e kratší cesta je prozkoumána døíve
		
//		global number_of_uavs params

		int sum = 0; // sum = celková délka všech vedoucích cest

		// delší cesta má vìtší prpst.proto, aby algoritmus asymptiticky pokryl kadou cestu stejnì hustì. na delší cestu tedy pøipadne více bodù.
		for (size_t i = 0; i < guiding_paths.size(); i++)
		{
			int pathSize = guiding_paths[i]->getSize(); // èím delší cesta, tím vìtší pravdìpodobnost, e se tou cestou vydá, aspoò doufám
			propabilities[i] = pathSize;		// propabilities je normální, 1D pole. nechápu, proè tady pouívá notaci pro 2D pole
			sum += pathSize;
		}
		
		for (size_t i = 0; i < guiding_paths.size(); i++)
		{
			propabilities[i] /= sum;
		}

		vector<shared_ptr<Point>> s_rand = vector<shared_ptr<Point>>(number_of_uavs);
		
		double random = Random::fromZeroToOne();
		if (random > guided_sampling_prob) //vybírá se náhodnı vzorek
		{
			for (size_t i = 0; i < number_of_uavs; i++)
			{
				if (goals_reached[i])
				{//todo: s tímhle nìco udìlat, a nepøistupovat k poli takhle teple pøes indexy
					s_rand[i] = random_state_goal(map->getGoals()[goals_reached[i]], map);	//pokud je n-té UAV v cíli, vybere se náhodnı bod z cílové plochy, kam UAV dorazilo
				} else
				{
					s_rand[i] = random_state(0, worldWidth, 0, worldHeight, map); // pokud n-té UAV není v cíli, vybere se náhodnı bod z celé mapy
				}
			}
		} else
		{
			//rozdìlit kvadrokoptéry na skupinky. Jedna skupina pro kadé AoI. Rozdìlit skupiny podle plochy, kterou jednotklivá AoI zabírají
			auto ratios = vector<double>(map->getGoals().size()); //pomìry jednotlivıch ploch ku celkové ploše. Dlouhé jako poèet cílù, tedy poèet guiding paths
			double totalVolume = 0;
			for (size_t i = 0; i < map->getGoals().size(); i++)
			{
				double volume = map->getGoals()[i]->rectangle->getVolume();
				ratios[i] = volume;
				totalVolume += volume;
			}
			for_each(ratios.begin(), ratios.end(), [totalVolume](double ratio) { return ratio /= totalVolume; });	//kadı prvek je v rozsahu od 0 do 1

			vector<shared_ptr<UavGroup>> uavGroups = vector<shared_ptr<UavGroup>>(map->getGoals().size());

			double uavsPerUnit = map->getUavsStart().size() / totalVolume;	// poèet UAV na jednotku celkové plochy. Po vynásobením plochou dané AoI získám poèet UAV na danou AoI.

			//pøerozdìlování kvadrokoptér podle pomìru inspirováno tímto https://github.com/sebastianbergmann/money/blob/master/src/Money.php#L261

			int uavsInGroups = 0;	//poèítá, kolik uav je rozvrenıch do skupin, kvùli zaokrouhlování
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
				uavGroups[i]->guidingPathIndex = i;
				uavsInGroups += uavsCountInGroup;
			}
			//rozházet do skupin nepøiøazená uav, která zbyla kvùli zaokrouhlování dolù
			int remaining = map->getUavsStart().size() - uavsInGroups;
			for (size_t i = 0; i < remaining; i++)	//zbytku je vdycky stejnì nebo ménì ne poètu skupin
			{
				uavGroups[i]->addUav(map->getUavsStart()[uavsInGroups + i], uavsInGroups + i);
			}

			for (size_t i = 0; i < uavGroups.size(); i++)
			{
				auto group = uavGroups[i];

				vector<int> groupCurrentIndexes;	//pole current indexù pro uav v dané group a pro cestu, kterou má daná group pøiøazenou.
				//pøed samotnım nalezením nejlepší dosaené node pro danou skupinu musím vytahat z matice current_index prvky, které potøebuji
				for (size_t i = 0; i < current_index.size(); i++)	//iteruji pøes všechna UAV a if urèí, zda je dané UAV ve skupinì, kterou zkoumám, èi ne
				{
					auto indexes = group->getUavIndexes();
					if (std::find(indexes.begin(), indexes.end(), i) != indexes.end()) {
						/* group->getUavIndexes() contains i */
						groupCurrentIndexes.push_back(current_index[i][group->guidingPathIndex]);
					}
					else {
						/* group->getUavIndexes() does not contain i */

					}
				}

				//teï je v groupCurrentIndexes current_index pro kadé UAV pro danou path z dané group
				double bestReachedIndex = *std::min_element(groupCurrentIndexes.begin(), groupCurrentIndexes.end());


				auto center = group->getGuidingPath()->get(bestReachedIndex);	//Petrlík má pro celou skupinu stejnı objekt center
				for (size_t j = 0; j < group->getUavs().size(); j++)
				{
					int index = group->getUavIndexes()[j];
					if (goals_reached[index])	//todo: zjistit, jestli tam nemá bıt index cíle
					{
						randomStates.push_back(random_state_goal(map->getGoals()[goals_reached[index]], map));
					}
					else
					{
						randomStates.push_back(random_state_polar(center->getPoint(), map, 0, configuration->getSamplingRadius()));
					}
				}
			}
		}
		return randomStates;
	}

	shared_ptr<State> Core::nearest_neighbor(vector<shared_ptr<Point>> s_rand, vector<shared_ptr<State>> nodes, int count)
	{
		int max_nodes = configuration->getRrtMaxNodes();
		int debug = configuration->getDebug();
		NNMethod nn_method = NNMethod::Total;

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
			shared_ptr<State> tmp_node = nodes[j];	//todo: refactorovat, aby se nesahalo do prázdnıch nodes

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
			case NNMethod::Total:
				for(auto dist : distances)
				{
					hamilt_dist += dist;	//no function for sum
				}
				break;
			case NNMethod::Max:
				hamilt_dist = *std::max_element(distances.begin(), distances.end());	//tohle vrací iterátor, kterı musím dereferencovat, abych získal èíslo. fuck you, C++
				break;
			case NNMethod::Min:
				hamilt_dist = *std::min_element(distances.begin(), distances.end());	//tohle vrací iterátor, kterı musím dereferencovat, abych získal èíslo. fuck you, C++
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
			
		if (near_arr.size() > count)	//todo: zjistit, zda je správnì nerovnost a zda nemá bıt >=
		{
			near_node = near_arr[near_arr.size() - count];	//indexuje se od 0, proto count musí zaèínat od 1
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

	vector<shared_ptr<State>> Core::select_input(vector<shared_ptr<Point>> s_rand, shared_ptr<State> near_node, shared_ptr<Map> map)
	{
		file << "Near node: " << *near_node.get() << endl;
		file << "s_rand";
		for (auto a : s_rand)
		{
			file << *a.get() << endl;
		}

		int input_samples_dist = configuration->getInputSamplesDist();
		int input_samples_phi = configuration->getInputSamplesPhi();
		int distance_of_new_nodes = configuration->getDistanceOfNewNodes();
		double max_turn = configuration->getMaxTurn();
		bool relative_localization = true;	//zatím natvrdo, protoe nevím, jak se má chovat druhá monost
		int uavCount = near_node->uavs.size();
		int inputCountPerUAV = input_samples_dist * input_samples_phi;
		int inputCount = configuration->getInputCount();
		vector<shared_ptr<Point>> oneUavInputs = vector<shared_ptr<Point>>();
		shared_ptr<State> new_node;

		for (size_t k = 0; k < input_samples_dist; k++)
		{
			for (size_t m = 0; m < input_samples_phi; m++)
			{
				double x = distance_of_new_nodes / pow(1.5,k);
				double y = -max_turn + 2 * m * max_turn / (input_samples_phi - 1);
				shared_ptr<Point> point = make_shared<Point>(x, y);
				oneUavInputs.push_back(point);
			}
		}

		//poèet všech monıch "kombinací" je variace s opakováním (n-tuple anglicky). 
		//inputs jsou vstupy do modelu
		vector<vector<shared_ptr<Point>>> inputs = generateNTuplet<shared_ptr<Point>>(oneUavInputs, uavCount);	//poèet všech kombinací je poèet všech monıch vstupù jednoho UAV ^ poèet UAV
		//translations jsou vıstupy z modelu
		vector<vector<shared_ptr<Point>>> translations = vector<vector<shared_ptr<Point>>>(inputCount);	//poèet všech kombinací je poèet všech monıch vstupù jednoho UAV ^ poèet UAV
		vector<shared_ptr<State>> tempStates = vector<shared_ptr<State>>(inputCount);	//poèet všech kombinací je poèet všech monıch vstupù jednoho UAV ^ poèet UAV

		for (size_t i = 0; i < inputs.size(); i++)
		{
			auto input = inputs[i];
			auto tempState = car_like_motion_model(near_node, input);	//this method changes near_node
			tempStates[i] = tempState;
			translations[i] = vector<shared_ptr<Point>>(tempState->uavs.size());
			for (size_t j = 0; j < tempState->uavs.size(); j++)
			{
				double x = tempState->uavs[j]->getLocation()->getX() - near_node->uavs[j]->getLocation()->getX();
				double y = tempState->uavs[j]->getLocation()->getY() - near_node->uavs[j]->getLocation()->getY();
				translations[i][j] = make_shared<Point>(x ,y);
			}
		}

		vector<double> d = vector<double>(inputCount);
		//todo: moná zrefactorovat a schovat do jednoho cyklu, kterı je vıše
		//todo: kde to pùjde, puít range-based loop, iteraci místo klasického foru
		//Distance to s_rand when using different inputs
		for (size_t i = 0; i < d.size(); i++)
		{
			auto tempState = tempStates[i];
			d[i] = 0;
			for (size_t j = 0; j < tempState->uavs.size(); j++)
			{
				double x = s_rand[j]->getX() - tempState->uavs[j]->getLocation()->getX();
				double y = s_rand[j]->getY() - tempState->uavs[j]->getLocation()->getY();
				d[i] += sqrt(pow(x, 2) + pow(y, 2));
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
					throw "No valid input left";
				}

				//todo: pokud mono, refactorovat, a nemusím minimum hledat ruènì
				int index = 0;	//klíè nejmenší hodnoty vzdálenosti v poli d
				double minValue = DBL_MAX;
				for (size_t i = 0; i < d.size(); i++)
				{
					if (d[i] < minValue)
					{
						minValue = d[i];
						index = i;
					}
				}
				auto tempState = tempStates[index];

				if (!check_localization_sep(tempState) || trajectory_intersection(near_node, tempState) || near_node->used_inputs[index])
				{
					d[index] = DBL_MAX; //jde o to vyøadit tuto hodnotu z hledání minima
					continue;
				}

				if (!check_world_bounds(tempState->uavs, configuration->getWorldWidth(), configuration->getWorldHeight()))
				{
					d[index] = DBL_MAX; //jde o to vyøadit tuto hodnotu z hledání minima
					continue;
				}

				near_node = check_obstacle_vcollide_single(near_node, translations, index, map);	//toto plní field used_inputs by true and thus new state looks like already used

				if (near_node->used_inputs[index])
				{
					d[index] = DBL_MAX; //jde o to vyøadit tuto hodnotu z hledání minima
					continue;
				} else
				{
					new_node = stateFactory->createState();
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
		
		if (!new_node)	//operator== urèuje, zda je pointer null èi ne
		{
			file.close();
			throw rrtPathError("No valid input found.");
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

	//todo: popøemıšlet na refactoringem current_index, abych tady nepracoval s maticí
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
		//todo: vymyslet, jak to udìlat, abych odsud nastavil parametry, ale nesahal na celou konfiguraci. udìlat asi nìjakı command na dìlení np_divisorem

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

	//only modifies node by inputs
	shared_ptr<State> Core::car_like_motion_model(shared_ptr<State> node, vector<shared_ptr<Point>> inputs)
	{
		auto newNode = make_shared<State>(*node.get());	//copy constructor, deep copy

		double uav_size = configuration->getUavSize();
		// Simulation step length
		double time_step = configuration->getTimeStep();
		// Simulation length
		double end_time = configuration->getEndTime();
		int number_of_uavs = node->uavs.size();
		//		global number_of_uavs params empty_trajectory
		
		//model parameters
		//front to rear axle distance
		double L = uav_size;
		
		//prepare trajectory array
		//repmat is time demanding
		auto trajectory = vector<shared_ptr<State>>();
		
		//main simulation loop
		//todo: všude, kde pouívám push_back se podívat, zda by nešlo na zaèátku naalokovat pole, aby se nemusela dynamicky mìnit velikost

		int count = 0;	//poèítadlo prùchodù
		for (double i = time_step; i < end_time; i += time_step)
		{
			count++;
			for (size_t j = 0; j < number_of_uavs; j++)
			{
				//calculate derivatives from inputs
				double dx = inputs[j]->getX() * cos(newNode->uavs[j]->getRotation()->getZ());	//pokud jsme ve 2D, pak jediná moná rotace je rotace okolo osy Z
				double dy = inputs[j]->getX() * sin(newNode->uavs[j]->getRotation()->getZ());	//input není klasickı bod se souøadnicemi X, Y, ale objekt se dvìma èísly, odpovídajícími dvìma vstupùm do car_like modelu
				double dPhi = (inputs[j]->getX() / L) * tan(inputs[j]->getY());

				//calculate current state variables
				newNode->uavs[j]->getLocation()->changeX(dx * time_step);
				newNode->uavs[j]->getLocation()->changeY(dy * time_step);
				newNode->uavs[j]->getRotation()->changeZ(dPhi * time_step);
			}
			newNode->prev_inputs = inputs;
			trajectory.push_back(newNode);
		}

		return newNode;
	}

	bool Core::check_localization_sep(shared_ptr<State> node)
	{
		int number_of_uavs = node->uavs.size();
		double relative_distance_min = configuration->getRelativeDistanceMin();
		double relative_distance_max = configuration->getRelativeDistanceMax();
		// Neighbor must be in certain angle on / off
		bool check_fov = false;
		double localization_angle = PI / 2;
		int required_neighbors = 1;
		bool allow_swarm_splitting = false;

		// Initialize default values
		vector<int> neighbors = vector<int>(number_of_uavs);
		fill(neighbors.begin(), neighbors.end(), 0);	//inicializace
		
		// Single UAV needs no localization
		if (number_of_uavs == 1)
		{
			return true;
		}
		
		// Check minimal distance between UAVs
		for (size_t i = 0; i < number_of_uavs - 1; i++)	//todo: zkontrolovat indexy, zda správnì sedí a neutíkají o 1
		{
			for (size_t j = i + 1; j < number_of_uavs; j++)
			{
				double uavIx = node->uavs[i]->getLocation()->getX();
				double uavIy = node->uavs[i]->getLocation()->getY();
				double uavJx = node->uavs[j]->getLocation()->getX();
				double uavJy = node->uavs[j]->getLocation()->getY();

				if (sqrt(pow(uavIx - uavJx, 2) + pow(uavIy - uavJy, 2)) <= relative_distance_min)
				{
					return false;
				}
			}
		}

		// Check maximal distance between UAVs
		for (size_t i = 0; i < number_of_uavs - 1; i++)	//todo: zkontrolovat indexy, zda správnì sedí a neutíkají o 1
		{
			for (size_t j = i + 1; j < number_of_uavs; j++)
			{
				double uavIx = node->uavs[i]->getLocation()->getX();
				double uavIy = node->uavs[i]->getLocation()->getY();
				double uavIphi = node->uavs[i]->getRotation()->getZ();
				double uavJx = node->uavs[j]->getLocation()->getX();
				double uavJy = node->uavs[j]->getLocation()->getY();
				double uavJphi = node->uavs[i]->getRotation()->getZ();

				if (sqrt(pow(uavIx - uavJx, 2) + pow(uavIy - uavJy, 2)) < relative_distance_max && (!check_fov || abs(uavIphi - uavJphi) < localization_angle / 2))
				{
					neighbors[i]++;
					neighbors[j]++;					
				}
			}
		}

		bool allUavsHaveNeighbors = false;
		bool oneOrMoreNeighbors = false;
		for (auto neighbor : neighbors)
		{
			allUavsHaveNeighbors = allUavsHaveNeighbors || neighbor >= required_neighbors;
			oneOrMoreNeighbors = oneOrMoreNeighbors || neighbor >= 1;
		}

		// Check whether each UAV has required number of neighbors
		// Pair formations
		if (allow_swarm_splitting)
		{
			return allUavsHaveNeighbors;
		} else
		// Whole swarm
		{
			int twoOrMoreNeighbors = 0;
			if (oneOrMoreNeighbors)
			{
//				printf("Neigbors: %d %d %d %d \n", neighbors[0], neighbors[1], neighbors[2], neighbors[3]);
				for (auto neighbor : neighbors)
				{
					neighbor > 1 ? twoOrMoreNeighbors++ : NULL;
				}
				return twoOrMoreNeighbors >= number_of_uavs - 2;
			}

		}
		return false;
	}

	bool Core::trajectory_intersection(shared_ptr<State> near_node, shared_ptr<State> tmp_node)
	{
		
		int number_of_uavs = near_node->uavs.size();
		for (size_t i = 0; i < number_of_uavs; i++)
		{
			for (size_t j = 0; j < number_of_uavs; j++)
			{
				if (i != j && line_segments_intersection(
					near_node->uavs[i]->getLocation(), 
					tmp_node->uavs[i]->getLocation(), 
					near_node->uavs[j]->getLocation(), 
					tmp_node->uavs[j]->getLocation()))
				{
					return true;
				}
			}
		}
		return false;
	}

	shared_ptr<State> Core::check_obstacle_vcollide_single(shared_ptr<State> near_node, vector<vector<shared_ptr<Point>>> translation, int index, shared_ptr<Map> map)
	{
		
//		global params number_of_uavs obstacles
		double uav_size = 0.5;
		int number_of_uavs = near_node->uavs.size();
		int number_of_obstacles = map->getObstacles().size();
		vector<bool> uavs_colliding = vector<bool>(number_of_uavs);
		fill(uavs_colliding.begin(), uavs_colliding.end(), false);
		double collision = false;
		
		vector<Triangle3D> tri_uav = vector<Triangle3D>(number_of_uavs);
		vector<Triangle3D> tri1_obs = vector<Triangle3D>(number_of_obstacles);
		vector<Triangle3D> tri2_obs = vector<Triangle3D>(number_of_obstacles);
		
		for (size_t i = 0; i < number_of_uavs; i++)
		{
			double x = near_node->uavs[i]->getLocation()->getX();
			double y = near_node->uavs[i]->getLocation()->getY();
			double x1 = x - uav_size / 2;
			double y1 = y - uav_size / 2;
			double z1 = 1;
			double x2 = x + uav_size / 2;
			double y2 = y - uav_size / 2;
			double z2 = 1;
			double x3 = x;
			double y3 = y + uav_size / 2;
			double z3 = 1;
			tri_uav[i] = Triangle3D(Point3D(x1, y1, z1), Point3D(x2, y2, z2), Point3D(x3, y3, z3));
		}
		
		double zero_trans[] = { 0,0,0, 1,0,0, 0,1,0, 0,0,1 };
		
		for (size_t i = 0; i < number_of_obstacles; i++)
		{
			auto obs = map->getObstacles()[i];
			Point3D p1 = Point3D(obs->rectangle->getX(), obs->rectangle->getY(), 1);
			Point3D p2 = Point3D(obs->rectangle->getX() + obs->rectangle->getWidth(), obs->rectangle->getY(), 1);
			Point3D p3 = Point3D(obs->rectangle->getX() + obs->rectangle->getWidth(), obs->rectangle->getY() + obs->rectangle->getHeight(), 1);
			Point3D p4 = Point3D(obs->rectangle->getX(), obs->rectangle->getY() + obs->rectangle->getHeight(), 1);

			tri1_obs[i] = Triangle3D(p1, p2, p3);
			tri2_obs[i] = Triangle3D(p1 ,p4, p3);
		}
		
		int k = index;
		for (size_t i = 0; i < number_of_uavs; i++)
		{
			for (size_t j = 0; j < number_of_obstacles; j++)
			{
				double trans[] = { translation[k][i]->getX(), translation[k][i]->getY(), 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 };
				bool col = ColDetect::coldetect(tri_uav[i], tri1_obs[j], trans, zero_trans);
				col = col || ColDetect::coldetect(tri_uav[i], tri2_obs[j], trans, zero_trans);
				if (col)
				{
					for (size_t l = 0; l < translation.size(); l++)
					{
						if (translation[l][i]->getY() == translation[k][i]->getY() && translation[l][i]->getX() == translation[k][i]->getX())
						{
							near_node->used_inputs[l] = true;
						}
					}
					near_node->used_inputs[k] = true;
					collision = true;
					uavs_colliding[i] = true;
				}
			}
		}

		//todo: collision a uavs_colliding se mají vracet, ale nikde se nepouøívají, take je smazat
		return near_node;
	}

	bool Core::line_segments_intersection(shared_ptr<Point> p1, shared_ptr<Point> p2, shared_ptr<Point> p3, shared_ptr<Point> p4)
	{
		auto p = line_line_intersection(p1, p2, p3, p4);
		
		if (isfinite(p->getX()) && isfinite(p->getY()))
		{
			if (line_point_intersection(p, p1, p2) &&
				line_point_intersection(p, p3, p4))
			{
				return true;
			}

		}
		return false;
	}

	bool Core::line_point_intersection(shared_ptr<Point> q, shared_ptr<Point> p1, shared_ptr<Point> p2)
	{
		double tolerance = 1e-10;

		double x = q->getX();
		double y = q->getY();
		double x1 = p1->getX();
		double y1 = p1->getY();
		double x2 = p2->getX();
		double y2 = p2->getY();

		double value = (pow(x - x1, 2) + pow(y - y1, 2)) + (pow(x - x2, 2) + pow(y - y2, 2)) - (pow(x1 - x2, 2) + pow(y1 - y2, 2));

		return value <= tolerance;
	}

	shared_ptr<Point> Core::line_line_intersection(shared_ptr<Point> p1, shared_ptr<Point> p2, shared_ptr<Point> p3, shared_ptr<Point> p4)
	{
		double x1 = p1->getX();
		double y1 = p1->getY();
		double x2 = p2->getX();
		double y2 = p2->getY();
		double x3 = p3->getX();
		double y3 = p3->getY();
		double x4 = p4->getX();
		double y4 = p4->getY();

		double px = ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4)) / 
			((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));
		double py = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4)) / 
			((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));

		return make_shared<Point>(px, py);
	}

	// Zde je java implementace, ze které vycházím https ://cs.wikipedia.org/wiki/Variace_(algoritmus)
	template<typename T> vector<vector<T>> Core::generateNTuplet(vector<T> usedChars, int tupletClass)
	{
		vector<vector<T>> list = vector<vector<T>>();	//todo: popøemıšlet, jak to refactorovat, aby se mohlo pracovat s fixní velikostí  pole
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
					row.push_back(character);
					list.push_back(row);
				}
			}
		}
		return list;
	}

}
