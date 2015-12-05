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
#include "Uav.h"
#include <valarray>
#include <algorithm>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#define PI 3.14159265358979323846

using namespace std;
using namespace boost::numeric;

namespace App
{

	Core::Core(shared_ptr<Configuration> configuration) :
		logger(make_shared<LoggerInterface>()), configuration(configuration), 
		stateFactory(make_shared<StateFactory>(configuration)),
		inputGenerator(make_shared<InputGenerator>(configuration->getInputSamplesDist(), configuration->getInputSamplesPhi()))
	{
		setLogger(make_shared<LoggerInterface>());	//I will use LoggerInterface as NilObject for Logger, because I am too lazy to write NilObject Class.

		MapFactory mapFactory;	//mapa se musí vygenerovat hned, aby se mohla vykreslit v gui, ale pøed spuštìním se musí pøekreslit
		maps = mapFactory.createMaps(configuration->getUavCount());	

		//todo: udìlat nìjakou inicializaci, která bude mimo kontruktor, abych i mohl zavolat vdy na zaèátku runu, aby se proèistily cache, apod.
	}


	Core::~Core()
	{
	}

	void Core::run()
	{
		
		clock_t start;
		double duration;

		start = clock();

		MapFactory mapFactory;
		maps = mapFactory.createMaps(configuration->getUavCount());	//mapy se musí generovat znovu, protoe se v nich generují starty uav, a ty se mohou mìnit podl ekonfigurace


		shared_ptr<Map> map = maps.at(configuration->getMapNumber());
		logger->logSelectedMap(map, configuration->getWorldWidth(), configuration->getWorldHeight());
		MapProcessor mapProcessor = MapProcessor(logger);	
		//nejdøíve potøebuji z cílù udìlat jeden shluk cílù jako jednolitou plochu a tomu najít støed. 
		//Celı roj pak má jen jednu vedoucí cestu, do støedu shluku. Pak se pomocí rrt roj rozmisuje v oblasti celého shluku
		auto nodes = mapProcessor.mapToNodes(map, configuration->getAStarCellSize(), configuration->getWorldWidth(), configuration->getWorldHeight(), configuration->getUavSize(), configuration->getAllowSwarmSplitting());
		GuidingPathFactory pathFactory = GuidingPathFactory(logger);
		auto paths = pathFactory.createGuidingPaths(nodes->getAllNodes(), nodes->getStartNode(), nodes->getEndNodes());

		duration = (clock() - start) / double(CLOCKS_PER_SEC);

		cout << to_string(duration) << "seconds to discretize map and find path" << endl;

		auto output = rrtPath(paths, configuration, map, nodes->getAllNodes());

		shared_ptr<State> lastState;
		if (output->goals_reached)
		{
			lastState = get_best_fitness(output->finalNodes, map);
		} else
		{
			//todo: narvat do outputu pole všech nodes, a ty sem dát místo allNodes.
			lastState = get_closest_node_to_goal(output->nodes, paths, map);
		}

		auto path = getPath(lastState);

		save_output();

	}

	void Core::testGui()
	{
		for (size_t i = 0; i < 200; i++)
		{
			this_thread::sleep_for(chrono::milliseconds(500));
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

	shared_ptr<Output> Core::rrtPath(vector<shared_ptr<Path>> guiding_paths, shared_ptr<Configuration> configuration, shared_ptr<Map> map, vector<shared_ptr<Node>> mapNodes)
	{
		int uavCount = configuration->getUavCount();
		int rrt_min_nodes = configuration->getRrtMinNodes();
		int rrt_max_nodes = configuration->getRrtMaxNodes();
		int number_of_solutions = configuration->getNumberOfSolutions();
		int near_count = configuration->getNearCount();
		bool debug = configuration->getDebug();
		double guiding_near_dist = configuration->getGuidingNearDist();

		cout << "Starting RRT-path...";

		vector<shared_ptr<State>> nodes = vector<shared_ptr<State>>();
		auto initialState = stateFactory->createState();
		initialState->uavs = map->getUavsStart();
		nodes.push_back(initialState);

		for (auto uav : initialState->uavs)
		{
			for (auto guidingPath : guiding_paths)
			{
				uav->current_indexes->set(guidingPath, guidingPath->get(0));
			}
		}

		auto final_nodes = vector<shared_ptr<State>>();

		//pøíprava mapy <stringová reprezentace bodu, node> pro rychlé urèování souèasné node
		auto nodesMap = std::map<string, shared_ptr<Node>>();	//todo: naplnit na zaèátku
		for (auto node : mapNodes)
		{
			nodesMap[node->getPoint()->toString()] = node;
		}

		shared_ptr<State> newState;
		shared_ptr<State> nearState = initialState;
		auto output = make_shared<Output>();

		int i = 0; // poèet expandovanıch nodes, hned na zaèátku se zvıší o jedna
		int m = 0; // poèet nalezenıch cest
		int s = 2; // poèet prùchodù cyklem ? prostì se to jen zvìtší o 1 pøi kadém prùchodu, nikde se nepouívá

		while ((m <= number_of_solutions || i < rrt_min_nodes) && i < rrt_max_nodes) // number_of_solutions je asi 10 000.
		{
			if (configuration->getStop())
			{
				break;
			}
			i++;	// initial node je 0. prvek, proto vkládám od 1

//			%Random state
			unordered_map<Uav, shared_ptr<Point>, UavHasher> s_rand = random_state_guided(guiding_paths, map, nearState); // vrátí pole náhodnıch bodù, jeden pro kadou kvadrokoptéru

			//Finding appropriate nearest neighbor
			int k = 0;	//poèítadlo nepouitelnıch nodes
			bool near_found = false;

			auto isNewUavPosition = false;
			//opakování, dokud nenajdu vyhovující øešení, poèítají se prùchody cyklem kvùli uvíznutí
			while (!near_found)
			{
				if (k > near_count)
				{
//					i--;	//todo: zjistit, proè se sniuje i o 1
//					throw "Not possible to find near node suitable for expansion";
					logger->logText("Not possible to find near node suitable for expansion");
				}
				nearState = nearest_neighbor(s_rand, nodes, k);

				newState = select_input(s_rand, nearState, map, nodesMap);
				// Vypadá to, e near_node je ve funkci select_input zmìnìná kvùli kontrole pøekáek

				bool allInputsUsed = nearState->areAllInputsUsed();

				auto isNearUavPosition = false;
				for (auto uavPosition : nearState->uavs)
				{
					isNearUavPosition = isNearUavPosition || uavPosition != false;
				}

				isNewUavPosition = newState != false;	//pointer je empty , pokud se pro UAV nenašla vhodná další pozice

				//poèítadlo uvíznutí. UAV uvízlo, pokud je tento if true
				if (allInputsUsed || !isNewUavPosition || !isNearUavPosition)	//kontrola empty new_node
				{
					k++;
					char buffer[1024];
					sprintf(buffer, "Skipping node, k++. allInputsUsed: %d , isNewUavPosition: %d, isNearUavPosition: %d", allInputsUsed, isNewUavPosition, isNearUavPosition);
					logger->logText(buffer);
					check_expandability(nodes);
				} else
				{
					near_found = true;
				}
			}

			if (!isNewUavPosition)	//spustí se v pøípadì, e se nedorazilo do cíle a nenašla se ádná cesta
			{
				check_expandability(nodes);
				logger->logText("NaN in new node");
				break;
			}

			newState->index = i;
			if (debug)
			{
				logger->logText("[debug] Added node index: " + to_string(newState->index));
			}

			if (i % 200 == 0)
			{
				char buffer[20];
				sprintf(buffer, "RRT size: %d", i);
				logger->logText(buffer);
			}

			nodes.push_back(newState);
			s++;

			guiding_point_reached(newState, guiding_paths, guiding_near_dist); // zde se uloí do current_index, kolik nodes zbıvá danému UAV do cíle
			check_near_goal(newState->uavs, map);

			output->distancesToGoal = vector<double>(nodes.size());
			if (newState->areUavsInGoals()) // pokud je nalezen cíl
			{
				output->goals_reached = newState->areUavsInGoals();
				output->finalNodes.push_back(newState);	//rekurzí se ze stavu dá získat celá cesta
				char buffer[1024];
				m++;
				sprintf(buffer, "%d viable paths found so far.", m);
				logger->logText(buffer);
			}
			output->distancesToGoal[i] = newState->distanceOfNewNodes;	//tohle dát do promìnné State, nastavit v select_input a pak to ze State tahat

			if (i % configuration->getDrawPeriod() == 0)
			{
				logger->logNewState(nearState, newState);
				logger->logRandomStates(s_rand);
			}
		}
		
		output->finalNodes.push_back(nodes[nodes.size() - 1]);	//poslední prvek
		check_near_goal(newState->uavs, map);
		output->goals_reached = newState->areUavsInGoals();
		//todo: ošetøit nodes a final_nodes proti nullpointerùm a vyházet null nody
		output->nodes = nodes;
		logger->logText("RRT-Path finished");
		return output;
	}

	unordered_map<Uav, shared_ptr<Point>, UavHasher> Core::random_state_guided(vector<shared_ptr<Path>> guiding_paths, shared_ptr<Map> map, shared_ptr<State> state)
	{
		double guided_sampling_prob = configuration->getGuidedSamplingPropability();
		int worldWidth = configuration->getWorldWidth();
		int worldHeight = configuration->getWorldHeight();
		unordered_map<Uav, shared_ptr<Point>, UavHasher> randomStates;

		double random = Random::fromZeroToOne();
		if (random > guided_sampling_prob) //vybírá se náhodnı vzorek
		{
			int index = 0;
			for (auto uav : state->uavs)
			{
				if (uav->isGoalReached())
				{//todo: s tímhle nìco udìlat, a nepøistupovat k poli takhle teple pøes indexy
					randomStates[*uav.get()] = random_state_goal(uav->getReachedGoal(), map);	//pokud je n-té UAV v cíli, vybere se náhodnı bod z cílové plochy, kam UAV dorazilo
				}
				else
				{
					randomStates[*uav.get()] = random_state(0, 0, worldWidth, worldHeight, map); // pokud n-té UAV není v cíli, vybere se náhodnı bod z celé mapy
				}
			}
			return randomStates;
		}
		else
		{
			vector<shared_ptr<UavGroup>> uavGroups = splitUavsToGroups(guiding_paths, map, state, configuration->getAllowSwarmSplitting());
			for (auto group : uavGroups)
			{
				//teï je v groupCurrentIndexes current_index pro kadé UAV pro danou path z dané group
				auto center = group->getBestNode();
				logger->logRandomStatesCenter(center->getPoint());
				for (auto uav : group->getUavs())
				{
					if (uav->isGoalReached())
					{
						randomStates[*uav.get()] = random_state_goal(uav->getReachedGoal(), map);
					}
					else
					{
						randomStates[*uav.get()] = random_state_polar(center->getPoint(), map, 0, configuration->getSamplingRadius());
					}
				}
			}

			//pøeskládání randomStates podle ID UAV.
			vector<int> uavIds = vector<int>(randomStates.size());
			unordered_map<Uav, shared_ptr<Point>, UavHasher> randomStatesSorted;
			int index = 0;
			for (auto pair : randomStates)
			{
				auto uav = pair.first;
				uavIds[index] = uav.getId();
				index++;
			}
			sort(uavIds.begin(), uavIds.end());
			for (int uavId : uavIds)
			{
				//nalezení uav s danım id
				for (auto pair : randomStates)
				{
					auto uav = pair.first;
					if (uav.getId() == uavId)
					{
						randomStatesSorted[uav] = randomStates[uav];
						break;
					}
				}
			}
			return randomStatesSorted;
		}
	}

	shared_ptr<State> Core::nearest_neighbor(unordered_map<Uav, shared_ptr<Point>, UavHasher> s_rand, vector<shared_ptr<State>> nodes, int count)
	{
		int max_nodes = configuration->getRrtMaxNodes();
		int debug = configuration->getDebug();
		NNMethod nn_method = configuration->getNearestNeighborMethod();

		vector<shared_ptr<State>> near_arr = vector<shared_ptr<State>>();
		shared_ptr<State> near_node;
		vector<tuple<double, shared_ptr<State>>> stateDistances;	//celková vzdálenost pro danı State, ukládám tam hamilt_dist, zatím pouze pro debug, nikde se nepouívá
		int s = 1;
		double current_best = DBL_MAX;
		
		for (int j = 0; j < nodes.size(); j++)
		{
			// Distance of next node in the tree
			shared_ptr<State> tmp_node = nodes[j];

			if (tmp_node->areAllInputsUsed())
			{
				char buffer[1024];
				sprintf(buffer, "Node %d is unexpandable", tmp_node->index);
				logger->logText(buffer);
				continue;
			}
			
			double hamilt_dist = 0;
			vector<double> distances = vector<double>(tmp_node->uavs.size());

			for (size_t i = 0; i < tmp_node->uavs.size(); i++)
			{
				auto uav = tmp_node->uavs[i];
				auto randomState = s_rand[*uav.get()];
				distances[i] = uav->getPointParticle()->getLocation()->getDistanceSquared(randomState);
			}

			switch (nn_method)
			{
			case NNMethod::Total:
				for(auto dist : distances)
				{
					hamilt_dist += dist;	//no function for sum, so I must do it by hand
				}
				break;
			case NNMethod::Max:
				hamilt_dist = *max_element(distances.begin(), distances.end());	//tohle vrací iterátor, kterı musím dereferencovat, abych získal èíslo. fuck you, C++
				break;
			case NNMethod::Min:
				hamilt_dist = *min_element(distances.begin(), distances.end());	//tohle vrací iterátor, kterı musím dereferencovat, abych získal èíslo. fuck you, C++
				break;
			}

			stateDistances.push_back(std::make_tuple(hamilt_dist, tmp_node));	//zde je celková vdálenost a stav, ke kterému se váe
			char buffer[1024];
			sprintf(buffer, "[debug] near node #%d found, distance to goal state: %f", tmp_node->index, hamilt_dist);
			logger->logText(buffer);

			sort(stateDistances.begin(), stateDistances.end(),
				[](const tuple<double, shared_ptr<State>>& a,
					const tuple<double, shared_ptr<State>>& b) -> bool
			{
				return std::get<0>(a) < std::get<0>(b);
			});
	
		}

		if (stateDistances.size() > count)
		{
			auto tuple = stateDistances[count];
			near_node = get<1>(tuple);
			double distance = get<0>(tuple);
			if (debug && count > 0)
			{
				char buffer[1024];
				sprintf(buffer, "[debug] near node #%d chosen, %d discarded, near node index %d, distance to goal state: %f", count, count, near_node->index, distance);
				logger->logText(buffer);
			}
		}

		if (!near_node)
		{
			throw "No suitable near node found";
		}

		return near_node;
	}

	shared_ptr<State> Core::select_input(unordered_map<Uav, shared_ptr<Point>, UavHasher> randomState, shared_ptr<State> near_node, shared_ptr<Map> map, std::map<string, shared_ptr<Node>> mapNodes)
	{
//		file << "Near node: " << *near_node.get() << endl;
//		file << "s_rand";
//		for (auto a : s_rand)
//		{
//			file << *a.get() << endl;
//		}

		int input_samples_dist = configuration->getInputSamplesDist();
		int input_samples_phi = configuration->getInputSamplesPhi();
		double max_turn = configuration->getMaxTurn();
		bool relative_localization = true;	//zatím natvrdo, protoe nevím, jak se má chovat druhá monost
		int uavCount = near_node->uavs.size();
		int inputCount = configuration->getInputCount();
		shared_ptr<State> newState;

		//todo: dodìlat. Sestavit mapu stringReprezentace pointu -> node, udìlat funkci na zaokrouhlování souøadnic (momentálího støedu všech uav), abych získal souøadnice bodu. Podle bohu v mapì najít nodu a tu tam poslat.
		
		Point uavMiddle(0, 0);
		for (auto uav : near_node->uavs)
		{
			uavMiddle.moveBy(uav->getPointParticle()->getLocation());
		}
		uavMiddle.setX(uavMiddle.getX() / uavCount);
		uavMiddle.setY(uavMiddle.getY() / uavCount);

		uavMiddle = roundToNodeCoords(uavMiddle);
		shared_ptr<Node> uavMiddleNode = mapNodes[uavMiddle.toString()];	//node, na které se nachází støed všech UAV
		double distance_of_new_nodes = getDistanceOfNewNodes(uavMiddleNode);

		//poèet všech monıch "kombinací" je variace s opakováním (n-tuple anglicky). 
		//inputs jsou vstupy do modelu, kombinace všech monıch vstupù (vstupy pro jedno uav se vygenerují vıše, jsou v oneUavInputs)
		auto inputs = inputGenerator->generateAllInputs(distance_of_new_nodes, max_turn, near_node->uavs);		//poèet všech kombinací je poèet všech monıch vstupù jednoho UAV ^ poèet UAV
																												//translations jsou vıstupy z modelu
		vector<unordered_map<Uav, shared_ptr<Point>, UavHasher>> translations = vector<unordered_map<Uav, shared_ptr<Point>, UavHasher>>(inputCount);	//poèet všech kombinací je poèet všech monıch vstupù jednoho UAV ^ poèet UAV
		vector<shared_ptr<State>> tempStates = vector<shared_ptr<State>>(inputCount);	//poèet všech kombinací je poèet všech monıch vstupù jednoho UAV ^ poèet UAV

		for (size_t i = 0; i < inputs.size(); i++)
		{
			auto input = inputs[i];
			auto tempState = car_like_motion_model(near_node, input);	//this method changes near_node
			tempStates[i] = tempState;
			for (auto uav : tempState->uavs)
			{
				double x = uav->getPointParticle()->getLocation()->getX() - near_node->getUav(uav)->getPointParticle()->getLocation()->getX();
				double y = uav->getPointParticle()->getLocation()->getY() - near_node->getUav(uav)->getPointParticle()->getLocation()->getY();
				translations[i][*uav.get()] = make_shared<Point>(x ,y);
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
			for (auto uav : tempState->uavs)
			{
				d[i] += randomState[*uav.get()]->getDistance(uav->getPointParticle()->getLocation());
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
//					throw "No valid input left";
					logger->logText("all inputs are used");
					break;	//vıjimka se nemá vyhazovat
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

				if (!check_localization_sep(tempState))
				{
					d[index] = DBL_MAX; //jde o to vyøadit tuto hodnotu z hledání minima
					logger->logText("check localization sep returned false");
					continue;
				}

				if (trajectory_intersection(near_node, tempState))
				{
					d[index] = DBL_MAX; //jde o to vyøadit tuto hodnotu z hledání minima
					logger->logText("trajectories intersect");
					continue;
				}

				if (near_node->used_inputs[index])
				{
					d[index] = DBL_MAX; //jde o to vyøadit tuto hodnotu z hledání minima
					logger->logText("input with index" + to_string(index) + "is used");
					continue;
				}

				if (!insideWorldBounds(tempState->uavs, configuration->getWorldWidth(), configuration->getWorldHeight()))
				{
					d[index] = DBL_MAX; //jde o to vyøadit tuto hodnotu z hledání minima
					logger->logText("out of world bounds");
					continue;
				}

				check_obstacle_vcollide_single(near_node, translations, index, map);	//this fills field used_inputs by true and thus new state looks like already used

				if (near_node->used_inputs[index])
				{
					d[index] = DBL_MAX; //jde o to vyøadit tuto hodnotu z hledání minima
					logger->logText("input with index" + to_string(index) + "is used after vcollide check");
					continue;
				} else
				{
					newState = stateFactory->createState();
					newState->uavs = tempState->uavs;
					newState->prev = near_node;
					newState->prev_inputs = tempState->prev_inputs;
					newState->index = tempState->index;
					newState->distanceOfNewNodes = distance_of_new_nodes;
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
		
		return newState;
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
			char buffer[1024];
			sprintf(buffer, "Not expandable nodes: %d/%d\n", unexpandable_count, nodes.size());
			logger->logText(buffer);
		}
		return unexpandable_count;
	}

	//detects narrow passage
	void Core::guiding_point_reached(shared_ptr<State> state, vector<shared_ptr<Path>> guiding_paths, double guiding_near_dist)
	{
		for (auto guiding_path : guiding_paths) {
			for (auto node : guiding_path->getNodes())	//todo: moná to pøedìlat a iterovat obrácenì, abych jel odzadu a udìlal break, kdy narazím na currentPoint u uav, abych nemusel kontrolovat isFirstCloserToEnd
			{
				for (auto uav : state->uavs)
				{
					bool reached = false;
					if (uav->getPointParticle()->getLocation()->getDistanceSquared(node->getPoint()) < pow(guiding_near_dist, 2))
					{
						reached = true;
						//Narrow passage detection
						detect_narrow_passage(node);
					}
					auto uavCurrentPoint = uav->current_indexes->get(guiding_path);
					if (reached && guiding_path->hasNext(uavCurrentPoint) && guiding_path->isFirstCloserOrSameToEnd(node, uavCurrentPoint)) //ošetøení, aby se UAV nevracela, a by nepøetekl index u guidingPath
					{
						uav->current_indexes->set(guiding_path, guiding_path->getNext(uavCurrentPoint));
						break;
					}
				}
			}
		}

	}

	void Core::check_near_goal(vector<shared_ptr<Uav>> uavs, shared_ptr<Map> map)
	{
		if (configuration->getAllowSwarmSplitting())
		{
			for (auto goal : map->getGoals())
			{
				for (auto uav : uavs)
				{
					if (goal->contains(uav->getPointParticle()->getLocation()))
					{
						uav->setReachedGoal(goal);
					}
				}
			}
		} else
		{
			for (auto uav : uavs)	//aby se pak uav rozmisovala v rámci celé plochy, která pokrıvá cíle
			{
				if (map->getGoalGroup()->contains(uav->getPointParticle()->getLocation()))
				{
					uav->setReachedGoal(map->getGoalGroup());
				}
			}
		}
	}

	void Core::detect_narrow_passage(shared_ptr<Node> node)
	{
		//todo: vymyslet, jak to udìlat, abych odsud nastavil parametry, ale nesahal na celou konfiguraci. udìlat asi nìjakı command na dìlení np_divisorem

//		global params defaults count
		if (node->getCost() > 1) {	//cost > 1 mají pøekáky a nody sousedící s pøekákami
			configuration->inNarrowPassage();
		} else {
			configuration->outsideNarrowPassage();
		}
	}

	shared_ptr<Point> Core::random_state_goal(shared_ptr<GoalInterface> goal, shared_ptr<Map> map)
	{
		shared_ptr<Point> random_state;
		do
		{
			random_state = goal->getRandomPointInside();
		} while (check_inside_obstacle(random_state, map));
		return random_state;
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
		} while (check_inside_obstacle(randomState, map) && insideWorldBounds(randomState, configuration->getWorldWidth(), configuration->getWorldHeight()));
		return randomState;
	}

	bool Core::insideWorldBounds(shared_ptr<Point> point, int worldWidth, int worldHeight)
	{
		bool inBounds = false;

		if (point->getX() < worldWidth && point->getX() > 0 && point->getY() < worldHeight && point->getY() > 0)
		{
			inBounds = true;
		}
		return inBounds;
	}

	bool Core::insideWorldBounds(vector<shared_ptr<Uav>> points, int worldWidth, int worldHeight)
	{
		bool inBounds = true;
		for (auto point : points)
		{
			inBounds = inBounds && insideWorldBounds(point->getPointParticle()->getLocation(), worldWidth, worldHeight);
		}
		return inBounds;
	}

	//only modifies node by inputs
	shared_ptr<State> Core::car_like_motion_model(shared_ptr<State> node, unordered_map<Uav, shared_ptr<Point>, UavHasher> inputs)
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

		for (auto uav : newNode->uavs)
		{
			auto uavPointParticle = uav->getPointParticle();
			double dPhi = (inputs[*uav.get()]->getX() / L) * tan(inputs[*uav.get()]->getY());	//dPhi se nemìní v rámci vnitøního cyklu, take staèí spošítat jen jednou

			for (double i = time_step; i < end_time; i += time_step)
			{
				//calculate derivatives from inputs
				double dx = inputs[*uav.get()]->getX() * cos(uavPointParticle->getRotation()->getZ());	//pokud jsme ve 2D, pak jediná moná rotace je rotace okolo osy Z
				double dy = inputs[*uav.get()]->getX() * sin(uavPointParticle->getRotation()->getZ());	//input není klasickı bod se souøadnicemi X, Y, ale objekt se dvìma èísly, odpovídajícími dvìma vstupùm do car_like modelu

				//calculate current state variables
				uavPointParticle->getLocation()->changeX(dx * time_step);
				uavPointParticle->getLocation()->changeY(dy * time_step);
				uavPointParticle->getRotation()->changeZ(dPhi * time_step);
			}
			newNode->prev_inputs = inputs;
			trajectory.push_back(newNode);
		}

		return newNode;
	}

	bool Core::check_localization_sep(shared_ptr<State> node)	//todo: zjistit, zda funguje správnì, pokud je nastaven 1 soused a vypnut swarm splitting
	{
		int number_of_uavs = node->uavs.size();
		double relative_distance_min = configuration->getRelativeDistanceMin();
		double relative_distance_max = configuration->getRelativeDistanceMax();
		bool check_fov = configuration->getCheckFov();
		// Neighbor must be in certain angle on / off
		double localization_angle = configuration->getLocalizationAngle();
		int required_neighbors = configuration->getRequiredNeighbors();
		bool allow_swarm_splitting = configuration->getAllowSwarmSplitting();

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
				auto uavI = node->uavs[i]->getPointParticle()->getLocation();
				auto uavJ = node->uavs[j]->getPointParticle()->getLocation();

				if (uavI->getDistance(uavJ) <= relative_distance_min)
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
				auto uavI = node->uavs[i]->getPointParticle()->getLocation();
				double uavIphi = node->uavs[i]->getPointParticle()->getRotation()->getZ();
				auto uavJ = node->uavs[j]->getPointParticle()->getLocation();
				double uavJphi = node->uavs[i]->getPointParticle()->getRotation()->getZ();

				if (uavI->getDistance(uavJ) < relative_distance_max && (!check_fov || abs(uavIphi - uavJphi) < localization_angle / 2))
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
//				char buffer[1024];
//				sprintf(buffer, "Neighbors: %d %d %d %d \n", neighbors[0], neighbors[1], neighbors[2], neighbors[3]);
//				logger->logText(buffer);
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
					near_node->uavs[i]->getPointParticle()->getLocation(),
					tmp_node->uavs[i]->getPointParticle()->getLocation(),
					near_node->uavs[j]->getPointParticle()->getLocation(),
					tmp_node->uavs[j]->getPointParticle()->getLocation()))
				{
					return true;
				}
			}
		}
		return false;
	}

	void Core::check_obstacle_vcollide_single(shared_ptr<State> near_node, vector<unordered_map<Uav, shared_ptr<Point>, UavHasher>> translation, int index, shared_ptr<Map> map)
	{
		double uav_size = configuration->getUavSize();
				
		double zero_trans[] = { 0,0,0, 1,0,0, 0,1,0, 0,0,1 };
				
		int k = index;
		for (auto uav : near_node->uavs)
		{
			double x = uav->getPointParticle()->getLocation()->getX();
			double y = uav->getPointParticle()->getLocation()->getY();
			double x1 = x - uav_size / 2;
			double y1 = y - uav_size / 2;
			double z1 = 1;
			double x2 = x + uav_size / 2;
			double y2 = y - uav_size / 2;
			double z2 = 1;
			double x3 = x;
			double y3 = y + uav_size / 2;
			double z3 = 1;
			Triangle3D tri_uav = Triangle3D(Point3D(x1, y1, z1), Point3D(x2, y2, z2), Point3D(x3, y3, z3));

			for (auto obs : map->getObstacles())
			{
				Point3D p1 = Point3D(obs->rectangle->getX(), obs->rectangle->getY(), 1);
				Point3D p2 = Point3D(obs->rectangle->getX() + obs->rectangle->getWidth(), obs->rectangle->getY(), 1);
				Point3D p3 = Point3D(obs->rectangle->getX() + obs->rectangle->getWidth(), obs->rectangle->getY() + obs->rectangle->getHeight(), 1);
				Point3D p4 = Point3D(obs->rectangle->getX(), obs->rectangle->getY() + obs->rectangle->getHeight(), 1);

				Triangle3D tri1_obs = Triangle3D(p1, p2, p3);
				Triangle3D tri2_obs = Triangle3D(p1, p4, p3);


				double trans[] = { translation[k][*uav.get()]->getX(), translation[k][*uav.get()]->getY(), 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 };
				bool col = ColDetect::coldetect(tri_uav, tri1_obs, trans, zero_trans);
				col = col || ColDetect::coldetect(tri_uav, tri2_obs, trans, zero_trans);
				if (col)
				{
					for (size_t l = 0; l < translation.size(); l++)
					{
						if (translation[l][*uav.get()]->getY() == translation[k][*uav.get()]->getY() && translation[l][*uav.get()]->getX() == translation[k][*uav.get()]->getX())
						{
							near_node->used_inputs[l] = true;
						}
					}
					near_node->used_inputs[k] = true;
				}
			}
		}
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

	double Core::getDistanceOfNewNodes(shared_ptr<Node> node)
	{
		double base = configuration->getDistanceOfNewNodes();
		return base * log(node->getDistanceToObstacle());	//èistì heuristicky vymyšlená funkce, aby to nezrychlovalo moc, kdy to bude dál
	}

	Point Core::roundToNodeCoords(Point point)
	{
		//"zaokrouhlí" bod na støed node
		int x = point.getX();
		int y = point.getY();
		x -= x % configuration->getAStarCellSize();
		x += (configuration->getAStarCellSize() / 2);
		y -= y % configuration->getAStarCellSize();
		y += (configuration->getAStarCellSize() / 2);
		return Point(x, y);
	}

	vector<shared_ptr<UavGroup>> Core::splitUavsToGroups(vector<shared_ptr<Path>> guiding_paths, shared_ptr<Map> map, shared_ptr<State> state, bool allowSwarmSplitting)
	{
		vector<shared_ptr<UavGroup>> uavGroups = vector<shared_ptr<UavGroup>>(guiding_paths.size());
		if (allowSwarmSplitting)
		{
			int number_of_uavs = map->getUavsStart().size();

			//rozdìlit kvadrokoptéry na skupinky. Jedna skupina pro kadé AoI. Rozdìlit skupiny podle plochy, kterou jednotklivá AoI zabírají
			//tohle je pro groups. 
			valarray<double> ratios = valarray<double>(guiding_paths.size()); //pomìry jednotlivıch ploch ku celkové ploše. Dlouhé jako poèet cílù, tedy poèet guiding paths
			double totalVolume = 0;
			for (size_t i = 0; i < map->getGoals().size(); i++)
			{
				double volume = map->getGoals()[i]->getRectangle()->getVolume();
				ratios[i] = volume;
				totalVolume += volume;
			}

			ratios /= totalVolume;	//valarray umoòuje vektorové operace, kadı prvek je v rozsahu od 0 do 1


									//pøerozdìlování kvadrokoptér podle pomìru inspirováno tímto https://github.com/sebastianbergmann/money/blob/master/src/Money.php#L261

			int uavsInGroups = 0;	//poèítá, kolik uav je rozvrenıch do skupin, kvùli zaokrouhlování
			for (size_t i = 0; i < uavGroups.size(); i++)
			{
				int uavsCountInGroup = floor(number_of_uavs * ratios[i]);	//round down
				auto uavs = vector<shared_ptr<Uav>>(uavsCountInGroup);
				for (size_t j = 0; j < uavsCountInGroup; j++)
				{
					uavs[j] = state->uavs[uavsInGroups + j];	//todo: tuhle èást asi zrefaktorovat. A nìkde mít objekty reprezentující uav, s jeho polohou, apod.
				}
				uavGroups[i] = make_shared<UavGroup>(uavs, guiding_paths[i]);
				uavsInGroups += uavsCountInGroup;
			}
			//rozházet do skupin nepøiøazená uav, která zbyla kvùli zaokrouhlování dolù
			int remaining = map->getUavsStart().size() - uavsInGroups;
			for (size_t i = 0; i < remaining; i++)	//zbytku je vdycky stejnì nebo ménì ne poètu skupin
			{
				uavGroups[i]->addUav(state->uavs[uavsInGroups + i]);
			}
		}
		else
		{
			uavGroups[0] = make_shared<UavGroup>(state->uavs, guiding_paths[0]);	//vím, e pøi této konfiguraci allowSwarmSplitting je pouze 1 guidingPath, všechna uav jsou v 1 skupinì
		}
		return uavGroups;
	}

	vector<shared_ptr<State>> Core::getPath(shared_ptr<State> last_node)
	{
		vector<shared_ptr<State>> path = vector<shared_ptr<State>>();
		auto iterNode = last_node;
		do
		{
			path.push_back(iterNode);
			iterNode = last_node->prev;
		} while (iterNode->prev);

		//todo: zjistit, zda potøebuji na nìco geo_path_length a další vìci, které jsou zakomentované
//		geo_path_length = 0;
//		for m = 2:length(path)
//			for n = 1 : number_of_uavs
//				geo_path_length = geo_path_length + ...
//				sqrt((path(m).loc(1, n) - path(m - 1).loc(1, n)) ^ 2 + ...
//					(path(m).loc(2, n) - path(m - 1).loc(2, n)) ^ 2);
//			end
//		end
//		geo_path_length = geo_path_length / number_of_uavs;
//
//		output.geometric_path_length = geo_path_length;
//		output.path = path;
//		output.runtime = nodes(end).time_added;
//		output.curvature = get_curvature(path);

		reverse(path.begin(), path.end());	//abych mìl cestu od zaèátku do konce
		return path;
	}

	shared_ptr<State> Core::get_best_fitness(vector<shared_ptr<State>> final_nodes, shared_ptr<Map> map)
	{
		auto finalStatesFitness = unordered_map<shared_ptr<State>, double>();
		for (auto finalState : final_nodes)
		{
			finalStatesFitness[finalState] = fitness_function(finalState, map);
		}

		//finding solution with best fitness
		pair<shared_ptr<State>, double> min = *min_element(finalStatesFitness.begin(), finalStatesFitness.end(), 
			[](pair<shared_ptr<State>, double> a, pair<shared_ptr<State>, double> b) {return a.second < b.second; }
		);
		return min.first;
	}

	double Core::fitness_function(shared_ptr<State> final_node, shared_ptr<Map> map)
	{
		int elementSize = configuration->getGoalElementSize();
		double initialValue = 100;
		double uavCameraX = (150 / elementSize);
		double uavCameraY = floor(100 / elementSize);
		double halfCameraX = floor(uavCameraX / 2);
		double halfCameraY = floor(uavCameraY / 2);
		double uavInitValue = 1/2;

		//do matice (vektoru vektorù) si budu ukládat hodnoty, jak uav vidí dané místo. matice je cíl diskretizovanı stejnì jako pøi a star hledání.
		//prázdnı cíl má hodnotu 100. pokud cíl vidí UAV, vydìlí se hodnota dvìma. Nejmenší souèet je nejlepší.
		//matice UAV má všude 1, jen tam, kam vidí UAV, je 0.5. Pak se prvek po prvku vynásobí s maticí cílù. tím se vydìlí dvìma to, co vidí UAV a zbytek je nedotèen.

		//inicializace matice cílù. Všechny cíle jsou v jedné matici
		int rowCount = floor(configuration->getWorldHeight() / elementSize);
		int columnCount = floor(configuration->getWorldWidth() / elementSize);
		auto goalMatrix = ublas::matrix<double>(rowCount, columnCount, 0);//initializes matrix with 0
		for (auto goal : map->getGoals())
		{
			
			//filling goal in matrix with initial value

			auto rect = goal->getRectangle();
			auto width = floor(rect->getWidth() / elementSize);
			auto height = floor(rect->getHeight() / elementSize);
			ublas::subrange(goalMatrix, rect->getX(), rect->getX() + width, rect->getY(), rect->getY() + height) = ublas::matrix<double>(width, height, initialValue);
		}

		//inicializace matic UAV
		auto uavMatrixes = unordered_map<Uav, ublas::matrix<double>, UavHasher>();
		for (auto uav : final_node->uavs)
		{
			uavMatrixes[*uav.get()] = ublas::matrix<double>(rowCount, columnCount, 1);//initializes matrix with 1
		}
		for (auto uavMatrix : uavMatrixes)
		{
			auto uav = uavMatrix.first;
			auto matrix = uavMatrix.second;

			//filling goal in matrix with initial value
			auto loc = uav.getPointParticle()->getLocation();
			ublas::subrange(matrix, loc->getX() - halfCameraX, loc->getX() + halfCameraX, loc->getY() - halfCameraY, loc->getY() + halfCameraY) = ublas::matrix<double>(uavCameraX, uavCameraY, uavInitValue);
		}

		//vytvoøení prùniku nenulovıch hodnot a úprava hodnot matice mapy
		for (auto uavMatrix : uavMatrixes)
		{
			auto uav = uavMatrix.first;
			auto matrix = uavMatrix.second;

			goalMatrix = element_prod(goalMatrix, matrix);
		}

		return sum(prod(ublas::scalar_vector<double>(goalMatrix.size1()), goalMatrix));	//sum of whole matrix, for weird reason, I must multiply here (prod), fuck you C++ http://stackoverflow.com/questions/24398059/how-do-i-sum-all-elements-in-a-ublas-matrix
	}

	shared_ptr<State> Core::get_closest_node_to_goal(vector<shared_ptr<State>> states, vector<shared_ptr<Path>> guiding_paths, shared_ptr<Map> map)
	{
//	function [ best_node, lowest_distance ] = get_closest_node_to_goal(  )
//	%GET_CLOSEST_NODE_TO_GOAL Summary of this function goes here
//	%   Detailed explanation goes here
//	
//	global nodes number_of_uavs goals
//	
//	lowest_distance = inf(length(goals),number_of_uavs);
//	best_node = nodes(1);
//	for m=1:length(nodes)
//	    for k=1:length(goals)
//	        for n=1:number_of_uavs
//	        distance(k,n) = sqrt((nodes(m).loc(1,n)-(goals{k}.x+goals{k}.width/2))^2 + ...
//	            (nodes(m).loc(2,n)-(goals{k}.y+goals{k}.height/2))^2);
//	        end
//	    end
//	    if sum(sum(distance))<sum(sum(lowest_distance))
//	        lowest_distance = distance;
//	        best_node=nodes(m);
//	    end
//	end
//	end
		vector<pair<shared_ptr<State>, double>> statesAndCosts = vector<pair<shared_ptr<State>, double>>();
		for (auto state : states)
		{
			double distance = 0;
			auto uavGroups = splitUavsToGroups(guiding_paths, map, state, configuration->getAllowSwarmSplitting());
			for (auto group : uavGroups)
			{
				for (auto uav : group->getUavs())
				{
					auto loc = uav->getPointParticle()->getLocation();
					auto goalLoc = group->getGuidingPath()->getGoal()->getMiddle();
					distance += sqrt(loc->getDistanceSquared(goalLoc));
				}
			}
			statesAndCosts.push_back(make_pair(state, distance));
		}

		//finding state nearest to goals
		pair<shared_ptr<State>, double> min = *min_element(statesAndCosts.begin(), statesAndCosts.end(),
			[](pair<shared_ptr<State>, double> a, pair<shared_ptr<State>, double> b) {return a.second < b.second; }
		);
		return min.first;
	}


	void Core::save_output()
	{
//		function[] = save_output()
//			% SAVE_OUTPUT Summary of this function goes here
//			%   Detailed explanation goes here
//
//			global params output
//
//			dir_path = strcat('output/', date);
//			mkdir(dir_path)
//			matfile = fullfile(dir_path, datestr(clock, 30));
//			data = struct('params', params, 'output', output); %#ok
//			save(matfile, 'data');
//		end
	}
}
