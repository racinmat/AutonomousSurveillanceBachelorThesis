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
#include "PathHandler.h"

using namespace std;
using namespace boost::numeric;

namespace App
{

	Core::Core(shared_ptr<Configuration> configuration) :
		logger(make_shared<LoggerInterface>()), configuration(configuration), 
		stateFactory(make_shared<StateFactory>(configuration)),
		inputGenerator(make_shared<InputGenerator>(configuration->getInputSamplesDist(), configuration->getInputSamplesPhi())),
		coverageResolver(make_shared<AoICoverageResolver>()), 
		distanceResolver(make_shared<DistanceResolver>(configuration)),
		motionModel(make_shared<CarLikeMotionModel>(configuration)), 
		collisionDetector(make_shared<CollisionDetector>(configuration)),
		persister(make_shared<Persister>())
	{
		pathHandler = make_shared<PathHandler>(collisionDetector);
		pathOptimizer = make_shared<PathOptimizer>(distanceResolver, configuration, motionModel, collisionDetector, logger);
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

		shared_ptr<LinkedState> lastState;
		if (output->goals_reached)
		{
			lastState = coverageResolver->get_best_fitness(output->finalNodes, map, configuration->getGoalElementSize(), configuration->getWorldWidth(), configuration->getWorldHeight());
		} else
		{
			//todo: narvat do outputu pole všech nodes, a ty sem dát místo allNodes.
			lastState = get_closest_node_to_goal(output->nodes, paths, map);
		}

		auto path = pathHandler->getPath(lastState);

		auto statePath = PathHandler::createStatePath(path);	//pøesype data do struktury, která má pouze vìci nezbytné pro Dubbinse a neplete tam zbyteènosti z rrt-path

		logger->logBestPath(statePath);
		persister->savePath(statePath);

//		statePath = pathHandler->straightenCrossingTrajectories(statePath);	//pokud se køíí trajektorie, pak nemohu optimalizovat
//
//		logger->logBestPath(statePath);

		statePath = pathOptimizer->optimizePath(statePath, map);

		logger->logBestPath(statePath, true);
		persister->savePath(statePath);
		persister->savePathToJson(statePath);

//		testGui();

		save_output();

	}

	void Core::testGui()
	{
		//testing kreslení UAV
		MapFactory mapFactory;
		maps = mapFactory.createMaps(configuration->getUavCount());	//mapy se musí generovat znovu, protoe se v nich generují starty uav, a ty se mohou mìnit podl ekonfigurace
		shared_ptr<Map> map = maps.at(configuration->getMapNumber());
		auto initialState = stateFactory->createState();
		initialState->setUavs(map->getUavsStart());
		auto initState = make_shared<State>(*initialState.get());
		//inicializace finálního stavu
		auto lastState = make_shared<State>(*initialState.get());
		int i = 0;
		for (auto uav : lastState->getUavs())
		{
			uav->getPointParticle()->getLocation()->setX(700);
			uav->getPointParticle()->getLocation()->setY(700 + i);
			i += 30;
		}

		auto path = vector<shared_ptr<State>>();
		path.push_back(initState);
		path.push_back(lastState);

//		pathOptimizer->optimizePathPart(path, map);


//		for (size_t i = 0; i < 200; i++)
//		{
//			this_thread::sleep_for(chrono::milliseconds(500));
//			this->logger->logText(to_string(i));
//		}
	}

	void Core::setLogger(shared_ptr<LoggerInterface> logger)
	{
		this->logger = logger;
		pathOptimizer->setLogger(logger);
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

		vector<shared_ptr<LinkedState>> states = vector<shared_ptr<LinkedState>>();
		auto initialState = stateFactory->createState();
		initialState->setUavs(map->getUavsStart());
		states.push_back(initialState);

		for (auto uav : initialState->getUavs())
		{
			for (auto guidingPath : guiding_paths)
			{
				uav->getCurrentGuidingPathPositions()->set(guidingPath, guidingPath->get(0));
			}
		}

		auto final_nodes = vector<shared_ptr<LinkedState>>();

		//pøíprava mapy <stringová reprezentace bodu, node> pro rychlé urèování souèasné node
		auto nodesMap = std::map<string, shared_ptr<Node>>();	//todo: naplnit na zaèátku
		for (auto node : mapNodes)
		{
			nodesMap[node->getPoint()->toString()] = node;
		}

		shared_ptr<LinkedState> newState;
		shared_ptr<LinkedState> nearState = initialState;
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
					logger->logText("Not possible to find near node suitable for expansion");
					break;
				}
				nearState = nearest_neighbor(s_rand, states, k);

				newState = select_input(s_rand, nearState, map, nodesMap);
				// Vypadá to, e near_node je ve funkci select_input zmìnìná kvùli kontrole pøekáek

				bool allInputsUsed = nearState->areAllInputsUsed();

				auto isNearUavPosition = false;
				for (auto uavPosition : nearState->getUavs())
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
					check_expandability(states);
				} else
				{
					near_found = true;
				}
			}

			if (!isNewUavPosition)	//spustí se v pøípadì, e se nedorazilo do cíle a nenašla se ádná cesta
			{
				check_expandability(states);
				logger->logText("NaN in new node");
				break;
			}

			if (debug)
			{
				logger->logText("[debug] Added node index: " + to_string(newState->getIndex()));
			}

			if (i % 200 == 0)
			{
				char buffer[20];
				sprintf(buffer, "RRT size: %d", i);
				logger->logText(buffer);
			}

			states.push_back(newState);
			s++;

			guiding_point_reached(newState, guiding_paths, guiding_near_dist); // zde se uloí do current_index, kolik nodes zbıvá danému UAV do cíle
			check_near_goal(newState->getUavs(), map);

			output->distancesToGoal = vector<double>(states.size());
			if (newState->areUavsInGoals()) // pokud je nalezen cíl
			{
				output->goals_reached = newState->areUavsInGoals();
				output->finalNodes.push_back(newState);	//rekurzí se ze stavu dá získat celá cesta
				char buffer[1024];
				m++;
				sprintf(buffer, "%d viable paths found so far.", m);
				logger->logText(buffer);
			}
			output->distancesToGoal[i] = newState->getDistanceOfNewNodes();	//tohle dát do promìnné State, nastavit v select_input a pak to ze State tahat

			if (i % configuration->getDrawPeriod() == 0)
			{
				logger->logNewState(nearState, newState);
				logger->logRandomStates(s_rand);
			}
		}
		
		output->finalNodes.push_back(states[states.size() - 1]);	//poslední prvek
		check_near_goal(newState->getUavs(), map);
		output->goals_reached = newState->areUavsInGoals();
		//todo: ošetøit nodes a final_nodes proti nullpointerùm a vyházet null nody
		output->nodes = states;
		logger->logText("RRT-Path finished");
		return output;
	}

	unordered_map<Uav, shared_ptr<Point>, UavHasher> Core::random_state_guided(vector<shared_ptr<Path>> guiding_paths, shared_ptr<Map> map, shared_ptr<LinkedState> state)
	{
		double guided_sampling_prob = configuration->getGuidedSamplingPropability();
		int worldWidth = configuration->getWorldWidth();
		int worldHeight = configuration->getWorldHeight();
		unordered_map<Uav, shared_ptr<Point>, UavHasher> randomStates;

		double random = Random::fromZeroToOne();
		if (random > guided_sampling_prob) //vybírá se náhodnı vzorek
		{
			int index = 0;
			for (auto uav : state->getUavs())
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

	shared_ptr<LinkedState> Core::nearest_neighbor(unordered_map<Uav, shared_ptr<Point>, UavHasher> randomStates, vector<shared_ptr<LinkedState>> nodes, int count)
	{
		int max_nodes = configuration->getRrtMaxNodes();
		int debug = configuration->getDebug();

		vector<shared_ptr<LinkedState>> near_arr = vector<shared_ptr<LinkedState>>();
		shared_ptr<LinkedState> near_node;
		vector<tuple<double, shared_ptr<LinkedState>>> stateDistances;	//celková vzdálenost pro danı State, ukládám tam hamilt_dist, zatím pouze pro debug, nikde se nepouívá
		int s = 1;
		double current_best = DBL_MAX;
		
		for (int j = 0; j < nodes.size(); j++)
		{
			// Distance of next node in the tree
			shared_ptr<LinkedState> tmp_node = nodes[j];

			if (tmp_node->areAllInputsUsed())
			{
				char buffer[1024];
				sprintf(buffer, "Node %d is unexpandable", tmp_node->getIndex());
				logger->logText(buffer);
				continue;
			}
			
			double totalDistance = distanceResolver->getDistance(tmp_node, randomStates);

			stateDistances.push_back(make_tuple(totalDistance, tmp_node));	//zde je celková vdálenost a stav, ke kterému se váe
			char buffer[1024];
			sprintf(buffer, "[debug] near node #%d found, distance to goal state: %f", tmp_node->getIndex(), totalDistance);
			logger->logText(buffer);

			sort(stateDistances.begin(), stateDistances.end(),
				[](const tuple<double, shared_ptr<LinkedState>>& a,
					const tuple<double, shared_ptr<LinkedState>>& b) -> bool
			{
				return get<0>(a) < get<0>(b);
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
				sprintf(buffer, "[debug] near node #%d chosen, %d discarded, near node index %d, distance to goal state: %f", count, count, near_node->getIndex(), distance);
				logger->logText(buffer);
			}
		}

		if (!near_node)
		{
			throw "No suitable near node found";
		}

		return near_node;
	}

	shared_ptr<LinkedState> Core::select_input(unordered_map<Uav, shared_ptr<Point>, UavHasher> randomState, shared_ptr<LinkedState> nearState, shared_ptr<Map> map, std::map<string, shared_ptr<Node>> mapNodes)
	{
		double max_turn = configuration->getMaxTurn();
		bool relative_localization = true;	//zatím natvrdo, protoe nevím, jak se má chovat druhá monost
		int uavCount = nearState->getUavs().size();
		int inputCount = configuration->getInputCount();
		shared_ptr<LinkedState> newState;

		//todo: dodìlat. Sestavit mapu stringReprezentace pointu -> node, udìlat funkci na zaokrouhlování souøadnic (momentálího støedu všech uav), abych získal souøadnice bodu. Podle bohu v mapì najít nodu a tu tam poslat.
		
		Point uavMiddle(0, 0);
		for (auto uav : nearState->getUavs())
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
		auto inputs = inputGenerator->generateAllInputs(distance_of_new_nodes, max_turn, nearState->getUavs());		//poèet všech kombinací je poèet všech monıch vstupù jednoho UAV ^ poèet UAV
		//translations jsou vıstupy z modelu
		vector<shared_ptr<LinkedState>> tempStates = vector<shared_ptr<LinkedState>>(inputCount);	//stavy, které jsou vıstupem všech vygenerovanıch vstupù do motion modelu
		
		for (size_t i = 0; i < inputs.size(); i++)
		{
			auto input = inputs[i];
			auto tempState = carLikeMotionModel(nearState, input);	//this method changes near_node
			tempStates[i] = tempState;
		}

		vector<double> d = vector<double>(inputCount);
		//todo: moná zrefactorovat a schovat do jednoho cyklu, kterı je vıše
		//todo: kde to pùjde, puít range-based loop, iteraci místo klasického foru
		//Distance to s_rand when using different inputs
		for (size_t i = 0; i < d.size(); i++)
		{
			auto tempState = tempStates[i];
			d[i] = 0;
			for (auto uav : tempState->getUavs())
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
				if (nearState->areAllInputsUsed())
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

				if (!collisionDetector->isStateValid(nearState, tempState, map))
				{
					d[index] = DBL_MAX; //jde o to vyøadit tuto hodnotu z hledání minima
					continue;
				}

				if (nearState->used_inputs[index])
				{
					d[index] = DBL_MAX; //jde o to vyøadit tuto hodnotu z hledání minima
					logger->logText("input with index" + to_string(index) + "is used");
					continue;
				}

				newState = stateFactory->createState(*tempState.get());
				newState->setPrevious(nearState);
				newState->setDistanceOfNewNodes(distance_of_new_nodes);
				nearState->used_inputs[index] = true;
				break;
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

	int Core::check_expandability(vector<shared_ptr<LinkedState>> nodes)
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
	void Core::guiding_point_reached(shared_ptr<LinkedState> state, vector<shared_ptr<Path>> guiding_paths, double guiding_near_dist)
	{
		for (auto guiding_path : guiding_paths) {
			for (auto node : guiding_path->getNodes())	//todo: moná to pøedìlat a iterovat obrácenì, abych jel odzadu a udìlal break, kdy narazím na currentPoint u uav, abych nemusel kontrolovat isFirstCloserToEnd
			{
				for (auto uav : state->getUavs())
				{
					bool reached = false;
					if (uav->getPointParticle()->getLocation()->getDistanceSquared(node->getPoint()) < pow(guiding_near_dist, 2))
					{
						reached = true;
						//Narrow passage detection
						detect_narrow_passage(node);
					}
					auto uavCurrentPoint = uav->getCurrentGuidingPathPositions()->get(guiding_path);
					if (reached && guiding_path->hasNext(uavCurrentPoint) && guiding_path->isFirstCloserOrSameToEnd(node, uavCurrentPoint)) //ošetøení, aby se UAV nevracela, a by nepøetekl index u guidingPath
					{
						uav->getCurrentGuidingPathPositions()->set(guiding_path, guiding_path->getNext(uavCurrentPoint));
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
		} while (check_inside_obstacle(randomState, map) && collisionDetector->insideWorldBounds(randomState, configuration->getWorldWidth(), configuration->getWorldHeight()));
		return randomState;
	}

	//only modifies node by inputs
	shared_ptr<LinkedState> Core::carLikeMotionModel(shared_ptr<LinkedState> state, unordered_map<Uav, shared_ptr<CarLikeControl>, UavHasher> inputs)
	{
		auto newNode = stateFactory->createState(*state.get());	//copy constructor is called, makes deep copy
		// Simulation length
		double end_time = configuration->getEndTime();
				
		//main simulation loop
		//todo: všude, kde pouívám push_back se podívat, zda by nešlo na zaèátku naalokovat pole, aby se nemusela dynamicky mìnit velikost

		for (auto uav : newNode->getUavs())
		{
			auto uavPointParticle = uav->getPointParticle();

			motionModel->calculateState(uavPointParticle, inputs[*uav.get()]);
			newNode->prev_inputs = inputs;
		}

		newNode->incrementTime(end_time);

		return newNode;
	}

	double Core::getDistanceOfNewNodes(shared_ptr<Node> node)
	{
		double base = configuration->getDistanceOfNewNodes();
		if (configuration->isSlowerMotionNearObstacles())
		{
			base *= log(node->getDistanceToObstacle());	//èistì heuristicky vymyšlená funkce, aby to nezrychlovalo moc, kdy to bude dál
		}
		return base;
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

	vector<shared_ptr<UavGroup>> Core::splitUavsToGroups(vector<shared_ptr<Path>> guiding_paths, shared_ptr<Map> map, shared_ptr<LinkedState> state, bool allowSwarmSplitting)
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
					uavs[j] = state->getUavs()[uavsInGroups + j];	//todo: tuhle èást asi zrefaktorovat. A nìkde mít objekty reprezentující uav, s jeho polohou, apod.
				}
				uavGroups[i] = make_shared<UavGroup>(uavs, guiding_paths[i]);
				uavsInGroups += uavsCountInGroup;
			}
			//rozházet do skupin nepøiøazená uav, která zbyla kvùli zaokrouhlování dolù
			int remaining = map->getUavsStart().size() - uavsInGroups;
			for (size_t i = 0; i < remaining; i++)	//zbytku je vdycky stejnì nebo ménì ne poètu skupin
			{
				uavGroups[i]->addUav(state->getUavs()[uavsInGroups + i]);
			}
		}
		else
		{
			uavGroups[0] = make_shared<UavGroup>(state->getUavs(), guiding_paths[0]);	//vím, e pøi této konfiguraci allowSwarmSplitting je pouze 1 guidingPath, všechna uav jsou v 1 skupinì
		}
		return uavGroups;
	}

	shared_ptr<LinkedState> Core::get_closest_node_to_goal(vector<shared_ptr<LinkedState>> states, vector<shared_ptr<Path>> guiding_paths, shared_ptr<Map> map)
	{
		vector<pair<shared_ptr<LinkedState>, double>> statesAndCosts = vector<pair<shared_ptr<LinkedState>, double>>();
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
		pair<shared_ptr<LinkedState>, double> min = *min_element(statesAndCosts.begin(), statesAndCosts.end(),
			[](pair<shared_ptr<LinkedState>, double> a, pair<shared_ptr<LinkedState>, double> b) {return a.second < b.second; }
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
