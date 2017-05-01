#include "Core.h"
#include "Map.h"
#include "Configuration.h"
#include "MapFactory.h"
#include "Path.h"
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
#include "UavForRRT.h"
#include <valarray>
#include <algorithm>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <cfloat>

#define PI 3.14159265358979323846
#include "PathHandler.h"
#include "CarLikeAnalyticMotionModel.h"
#include "EmptyPath.h"

using namespace std;
using namespace boost::numeric;

namespace App
{

	Core::Core(shared_ptr<Configuration> configuration) :
		logger(make_shared<LoggerInterface>()), configuration(configuration), 
		stateFactory(make_shared<StateFactory>(configuration)),
		inputGenerator(make_shared<InputGenerator>(configuration->getInputSamplesDist(), configuration->getInputSamplesPhi())),
		coverageResolver(make_shared<AoICoverageResolver>(configuration)), 
		distanceResolver(make_shared<DistanceResolver>(configuration)),
		motionModel(make_shared<CarLikeAnalyticMotionModel>(configuration, logger)),
		collisionDetector(make_shared<CollisionDetector>(configuration)),
		persister(make_shared<Persister>(configuration, distanceResolver)),
		guidingPathFactory(make_shared<GuidingPathFactory>(logger)),
		resampler(make_shared<Resampler>(configuration, stateFactory, motionModel)),
		mapProcessor(make_shared<MapProcessor>(logger, configuration))
	{
		pathHandler = make_shared<PathHandler>(collisionDetector);
		pathOptimizer = make_shared<PathOptimizer>(distanceResolver, configuration, motionModel, collisionDetector, logger, persister, resampler);
		setLogger(make_shared<LoggerInterface>());	//I will use LoggerInterface as NilObject for Logger, because I am too lazy to write NilObject Class.

		MapFactory mapFactory;	//mapa se mus� vygenerovat hned, aby se mohla vykreslit v gui, ale p�ed spu�t�n�m se mus� p�ekreslit
		maps = mapFactory.createMaps(configuration->getUavCount());	
		//todo: ud�lat n�jakou inicializaci, kter� bude mimo kontruktor, abych i mohl zavolat v�dy na za��tku runu, aby se pro�istily cache, apod.
	}


	Core::~Core()
	{
	}

	void Core::run()
	{
//		clock_t start;
//		double duration;
//		start = clock();

		MapFactory mapFactory;
		maps = mapFactory.createMaps(configuration->getUavCount());	//mapy se mus� generovat znovu, proto�e se v nich generuj� starty uav, a ty se mohou m�nit podl ekonfigurace


		shared_ptr<Map> map = (shared_ptr<Map> &&) maps.at((unsigned int) configuration->getMapNumber());


		//nejd��ve pot�ebuji z c�l� ud�lat jeden shluk c�l� jako jednolitou plochu a tomu naj�t st�ed. 
		//Cel� roj pak m� jen jednu vedouc� cestu, do st�edu shluku. Pak se pomoc� rrt roj rozmis�uje v oblasti cel�ho shluku

		logger->logSelectedMap(map, configuration->getWorldWidth(), configuration->getWorldHeight());

		map->amplifyObstacles(configuration->getObstacleIncrement());

		vector<shared_ptr<State>> statePath = runRRTPath();

		logger->logBestPath(statePath);
		persister->savePathToJson(statePath, map, "before-dubins");
//		persister->writePathData(statePath);


		//instead of searching the way, I use saved way to be optimized
//		auto tuple = persister->loadPathFromJson("C:\\Users\\Azathoth\\Documents\\visual studio 2015\\Projects\\SwarmDeployment\\Win32\\Release\\path-02-28-20-25-16-before-dubins.json");
//
//		vector<shared_ptr<State>> statePath;
//		shared_ptr<Map> loadedMap;
//		tie(statePath, loadedMap) = tuple;


		statePath = resampler->resampleToMaxFrequency(statePath);
		persister->savePathToJson(statePath, map, "resampled");

		statePath = pathOptimizer->optimizePathByDubins(statePath, map);
		statePath = pathOptimizer->removeDuplicitStates(statePath);

		logger->logBestPath(statePath, true);
		logger->saveVisualMap();
		persister->savePath(statePath);
		persister->savePathToJson(statePath, map, "optimized");
		persister->savePathToCsv(statePath, "optimized");

//		testGui();

	}

	vector<shared_ptr<State>> Core::runRRTPath()
	{
		MapFactory mapFactory;
		maps = mapFactory.createMaps(configuration->getUavCount());	//mapy se mus� generovat znovu, proto�e se v nich generuj� starty uav, a ty se mohou m�nit podl ekonfigurace


		shared_ptr<Map> map = (shared_ptr<Map> &&) maps.at((unsigned int) configuration->getMapNumber());
		//nejd��ve pot�ebuji z c�l� ud�lat jeden shluk c�l� jako jednolitou plochu a tomu naj�t st�ed. 
		//Cel� roj pak m� jen jednu vedouc� cestu, do st�edu shluku. Pak se pomoc� rrt roj rozmis�uje v oblasti cel�ho shluku

		logger->logSelectedMap(map, configuration->getWorldWidth(), configuration->getWorldHeight());

		map->amplifyObstacles(configuration->getObstacleIncrement());

		auto nodes = mapProcessor->mapToNodes(map);
		auto paths = guidingPathFactory->createGuidingPaths(nodes->getAllNodes(), nodes->getStartNode(), nodes->getEndNodes());
		//			duration = (clock() - start) / double(CLOCKS_PER_SEC);
		//
		//			cout << to_string(duration) << "seconds to discretize map and find path" << endl;


		auto output = rrtPath(paths, configuration, map, nodes->getAllNodes());

		shared_ptr<LinkedState> lastState;
		if (output->goals_reached || configuration->getPlacementMethod() == PlacementMethod::Chain)	//u PlacementMethod::Chain nemus� v�echna UAV dorazit do c�le
		{
			lastState = coverageResolver->get_best_fitness(output->finalNodes, map);
		}
		else
		{
			lastState = get_closest_node_to_goal(output->nodes, paths, map);
		}

		auto path = pathHandler->getPath(lastState);

		auto statePath = PathHandler::createStatePath(path);
		persister->writePathData(statePath);
		return statePath;	//p�esype data do struktury, kter� m� pouze v�ci nezbytn� pro Dubinse a neplete tam zbyte�nosti z rrt-path
	}

	void Core::testGui()
	{
//		//testing kreslen� UAV
//		MapFactory mapFactory;
//		maps = mapFactory.createMaps(configuration->getUavCount());	//mapy se mus� generovat znovu, proto�e se v nich generuj� starty uav, a ty se mohou m�nit podl ekonfigurace
//		shared_ptr<Map> map = maps.at(configuration->getMapNumber());
//		auto initialState = stateFactory->createState();
//		initialState->setUavs(map->getUavsStart());
//		auto initState = make_shared<State>(*initialState.get());
//		//inicializace fin�ln�ho stavu
//		auto lastState = make_shared<State>(*initialState.get());
//		int i = 0;
//		for (auto uav : lastState->getBaseUavs())
//		{
//			uav->getPointParticle()->getLocation()->setX(700);
//			uav->getPointParticle()->getLocation()->setY(700 + i);
//			i += 30;
//		}
//
//		auto path = vector<shared_ptr<State>>();
//		path.push_back(initState);
//		path.push_back(lastState);
//
////		pathOptimizer->optimizePathPart(path, map);
//
//
////		for (size_t i = 0; i < 200; i++)
////		{
////			this_thread::sleep_for(chrono::milliseconds(500));
////			this->logger->logText(to_string(i));
////		}
	}

	void Core::setLogger(shared_ptr<LoggerInterface> logger)
	{
		this->logger = logger;
		pathOptimizer->setLogger(logger);
		motionModel->setLogger(logger);
		collisionDetector->setLogger(logger);
		mapProcessor->setLogger(logger);
		guidingPathFactory->setLogger(logger);
	}

	void Core::logConfigurationChange()
	{
		auto map = maps.at(configuration->getMapNumber());
		logger->logSelectedMap(map, configuration->getWorldWidth(), configuration->getWorldHeight());
	}

	shared_ptr<Output> Core::rrtPath(vector<shared_ptr<Path>> guiding_paths, shared_ptr<Configuration> configuration, shared_ptr<Map> map, vector<shared_ptr<Node>> mapNodes)
	{
		int rrt_min_nodes = configuration->getRrtMinNodes();
		int rrt_max_nodes = configuration->getRrtMaxNodes();
		int number_of_solutions = configuration->getNumberOfSolutions();
		int near_count = configuration->getNearCount();
		bool debug = configuration->getDebug();
		double guiding_near_dist = configuration->getGuidingNearDist();

		cout << "Starting RRT-path..." << endl;

		vector<shared_ptr<LinkedState>> states = vector<shared_ptr<LinkedState>>();
		auto initialState = stateFactory->createState();
		initialState->setUavs(map->getUavsStart());
		states.push_back(initialState);

		if (!collisionDetector->isInitialSwarmStateFeasible(initialState))
		{
			throw "Initial state in chosen map is not feasible.";
		}


		for (auto uav : initialState->getUavsForRRT())
		{
			for (auto guidingPath : guiding_paths)
			{
				uav->getCurrentGuidingPathPositions()->set(guidingPath, guidingPath->get(0));
			}
		}

		auto final_nodes = vector<shared_ptr<LinkedState>>();

		//p��prava mapy <stringov� reprezentace bodu, node> pro rychl� ur�ov�n� sou�asn� node
		auto nodesMap = std::map<string, shared_ptr<Node>>();	//todo: naplnit na za��tku
		for (auto node : mapNodes)
		{
			nodesMap[node->getPoint()->toString()] = node;
		}

		shared_ptr<LinkedState> newState;
		shared_ptr<LinkedState> nearState = initialState;
		auto output = make_shared<Output>();

		int i = 0; // po�et expandovan�ch nodes, hned na za��tku se zv��� o jedna
		int m = 0; // po�et nalezen�ch cest

		while ((m <= number_of_solutions || i < rrt_min_nodes) && i < rrt_max_nodes) // number_of_solutions je asi 10 000.
		{
			if (configuration->getStop())
			{
				break;
			}

			if (configuration->getMaxFinalNodes() < output->finalNodes.size())
			{
				break;
			}

			i++;	// initial node je 0. prvek, proto vkl�d�m od 1

//			%Random state
			unordered_map<UavForRRT, shared_ptr<Point>, UavHasher> s_rand = random_state_guided(guiding_paths, map, nearState); // vr�t� pole n�hodn�ch bod�, jeden pro ka�dou kvadrokopt�ru

			//Finding appropriate nearest neighbor
			int k = 0;	//po��tadlo nepou�iteln�ch nodes
			bool near_found = false;

			auto isNewUavPosition = false;
			//opakov�n�, dokud nenajdu vyhovuj�c� �e�en�, po��taj� se pr�chody cyklem kv�li uv�znut�
			while (!near_found)
			{
				if (k > near_count)
				{
					logger->logText("Not possible to find near node suitable for expansion");
					break;
				}
				nearState = nearest_neighbor(s_rand, states, k);

				newState = select_input(s_rand, nearState, map, nodesMap);
				// Vypad� to, �e near_node je ve funkci select_input zm�n�n� kv�li kontrole p�ek�ek

				bool allInputsUsed = nearState->areAllInputsUsed();

				auto isNearUavPosition = false;
				for (auto uavPosition : nearState->getBaseUavs())
				{
					isNearUavPosition = isNearUavPosition || uavPosition != false;
				}

				isNewUavPosition = newState != false;	//pointer je empty , pokud se pro UAV nena�la vhodn� dal�� pozice

				//po��tadlo uv�znut�. UAV uv�zlo, pokud je tento if true
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

			if (!isNewUavPosition)	//spust� se v p��pad�, �e se nedorazilo do c�le a nena�la se ��dn� cesta
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

			checkSmartZeroStepEnabling(newState, map);

			guiding_point_reached(newState, guiding_paths, guiding_near_dist); // zde se ulo�� do current_index, kolik nodes zb�v� dan�mu UAV do c�le
			check_near_goal(newState->getUavsForRRT(), map);

			output->distancesToGoal = vector<double>(states.size());
			if (newState->areUavsInGoals()) // pokud je nalezen c�l
			{
				output->goals_reached = newState->areUavsInGoals();
				output->finalNodes.push_back(newState);	//rekurz� se ze stavu d� z�skat cel� cesta
				char buffer[1024];
				m++;
				sprintf(buffer, "%d viable paths found so far.", m);
				logger->logText(buffer);
			}
			output->distancesToGoal[i] = newState->getDistanceOfNewNodes();	//tohle d�t do prom�nn� State, nastavit v select_input a pak to ze State tahat

			if (i % configuration->getDrawPeriod() == 0)
			{
				logger->logNewState(nearState, newState);
				logger->logRandomStates(s_rand);
			}
		}
		
		output->finalNodes.push_back(states[states.size() - 1]);	//posledn� prvek
		check_near_goal(newState->getUavsForRRT(), map);
		output->goals_reached = newState->areUavsInGoals();
		output->nodes = states;
		logger->logText("RRT-Path finished");
		return output;
	}

	void Core::loadAndOptimizeByDubins(string filename, vector<double> frequencies)
	{
		MapFactory mapFactory;
		maps = mapFactory.createMaps(configuration->getUavCount());	//mapy se mus� generovat znovu, proto�e se v nich generuj� starty uav, a ty se mohou m�nit podle konfigurace
		shared_ptr<Map> map = maps.at(configuration->getMapNumber());
		map->amplifyObstacles(configuration->getObstacleIncrement());

		auto tuple = persister->loadPathFromJson(filename);
		
		vector<shared_ptr<State>> statePathOriginal;
		vector<shared_ptr<State>> statePath;
		tie(statePathOriginal, map) = tuple;

		for(auto frequency : frequencies)
		{
			configuration->setMaxSampleFrequency(frequency);

			statePath = resampler->resampleToMaxFrequency(statePathOriginal);
//			persister->savePathToJson(statePath, map, "resampled");
//
			statePath = pathOptimizer->optimizePathByDubins(statePath, map);
			statePath = pathOptimizer->removeDuplicitStates(statePath);

//			persister->savePath(statePath);
			persister->savePathToJson(statePath, map, "optimized");
//			persister->savePathToCsv(statePath, "optimized");
			cout << "frequency " + to_string(frequency) + " completed" << endl;
		}

	}

	unordered_map<UavForRRT, shared_ptr<Point>, UavHasher> Core::random_state_guided(vector<shared_ptr<Path>> guiding_paths, shared_ptr<Map> map, shared_ptr<LinkedState> state)
	{
		double guided_sampling_prob = configuration->getGuidedSamplingPropability();
		int worldWidth = configuration->getWorldWidth();
		int worldHeight = configuration->getWorldHeight();
		unordered_map<UavForRRT, shared_ptr<Point>, UavHasher> randomStates;

		double random = Random::fromZeroToOne();
		if (random > guided_sampling_prob) //vyb�r� se n�hodn� vzorek
		{
			int index = 0;
			for (auto uav : state->getUavsForRRT())
			{
				if (uav->isGoalReached())
				{//todo: s t�mhle n�co ud�lat, a nep�istupovat k poli takhle teple p�es indexy
					randomStates[*uav.get()] = random_state_goal(uav->getReachedGoal(), map);	//pokud je n-t� UAV v c�li, vybere se n�hodn� bod z c�lov� plochy, kam UAV dorazilo
				}
				else
				{
					randomStates[*uav.get()] = random_state(0, 0, worldWidth, worldHeight, map); // pokud n-t� UAV nen� v c�li, vybere se n�hodn� bod z cel� mapy
				}
			}
			return randomStates;
		}
		else
		{
			vector<shared_ptr<UavGroup>> uavGroups = splitUavsToGroups(guiding_paths, map, state, configuration->getAllowSwarmSplitting());
			for (auto group : uavGroups)
			{
				//te� je v groupCurrentIndexes current_index pro ka�d� UAV pro danou path z dan� group
				for (auto uav : group->getUavs())
				{
					if (uav->isGoalReached())		// v p��pad� PlacementMethod::Chain se toto u UAV, kter� nejsou krajn�, spust� v�dy
					{
						randomStates[*uav.get()] = random_state_goal(uav->getReachedGoal(), map);
					}
					else
					{
						auto center = group->getBestNode();	//v p��pad� PlacementMethod::Chain 
						logger->logRandomStatesCenter(center->getPoint());
						randomStates[*uav.get()] = random_state_polar(center->getPoint(), map, 0, configuration->getSamplingRadius());
					}
				}
			}

			//p�eskl�d�n� randomStates podle ID UAV.
			vector<int> uavIds = vector<int>(randomStates.size());
			unordered_map<UavForRRT, shared_ptr<Point>, UavHasher> randomStatesSorted;
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
				//nalezen� uav s dan�m id
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

	shared_ptr<LinkedState> Core::nearest_neighbor(unordered_map<UavForRRT, shared_ptr<Point>, UavHasher> randomStates, vector<shared_ptr<LinkedState>> nodes, int count)
	{
		int max_nodes = configuration->getRrtMaxNodes();
		int debug = configuration->getDebug();

		vector<shared_ptr<LinkedState>> near_arr = vector<shared_ptr<LinkedState>>();
		shared_ptr<LinkedState> near_node;
		vector<tuple<double, shared_ptr<LinkedState>>> stateDistances;	//celkov� vzd�lenost pro dan� State, ukl�d�m tam hamilt_dist, zat�m pouze pro debug, nikde se nepou��v�
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

			stateDistances.push_back(make_tuple(totalDistance, tmp_node));	//zde je celkov� vd�lenost a stav, ke kter�mu se v�e
			char buffer[1024];
			if (debug)
			{
				sprintf(buffer, "[debug] near node #%d found, distance to goal state: %f", tmp_node->getIndex(), totalDistance);
				logger->logText(buffer);
			}

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

	shared_ptr<LinkedState> Core::select_input(unordered_map<UavForRRT, shared_ptr<Point>, UavHasher> randomState, shared_ptr<LinkedState> nearState, shared_ptr<Map> map, std::map<string, shared_ptr<Node>> mapNodes)
	{
		double max_turn = configuration->getMaxTurn();
		bool relative_localization = true;	//zat�m natvrdo, proto�e nev�m, jak se m� chovat druh� mo�nost
		int uavCount = nearState->getBaseUavs().size();
		int inputCount = configuration->getInputCount();
		bool debug = configuration->getDebug();
		shared_ptr<LinkedState> newState;

		//todo: dod�lat. Sestavit mapu stringReprezentace pointu -> node, ud�lat funkci na zaokrouhlov�n� sou�adnic (moment�l�ho st�edu v�ech uav), abych z�skal sou�adnice bodu. Podle bohu v map� naj�t nodu a tu tam poslat.
		
		Point uavMiddle(0, 0);
		for (auto uav : nearState->getBaseUavs())
		{
			uavMiddle.moveBy(uav->getPointParticle()->getLocation());
		}
		uavMiddle.setX(uavMiddle.getX() / uavCount);
		uavMiddle.setY(uavMiddle.getY() / uavCount);

		uavMiddle = roundToNodeCoords(uavMiddle);
		shared_ptr<Node> uavMiddleNode = mapNodes[uavMiddle.toString()];	//node, na kter� se nach�z� st�ed v�ech UAV
		double distance_of_new_nodes = getDistanceOfNewNodes(uavMiddleNode);

		//po�et v�ech mo�n�ch "kombinac�" je variace s opakov�n�m (n-tuple anglicky). 
		//inputs jsou vstupy do modelu, kombinace v�ech mo�n�ch vstup� (vstupy pro jedno uav se vygeneruj� v��e, jsou v oneUavInputs)
		auto inputs = inputGenerator->generateAllInputs(distance_of_new_nodes, max_turn, nearState->getUavsForRRT(), configuration->getZeroStepEnabled());		//po�et v�ech kombinac� je po�et v�ech mo�n�ch vstup� jednoho UAV ^ po�et UAV
		//tempStates jsou v�stupy z modelu, input se stejn�m indexem jako tempState je vstup do motion modelu, kter� vede na tempState
		vector<shared_ptr<LinkedState>> tempStates = vector<shared_ptr<LinkedState>>(inputCount);	//stavy, kter� jsou v�stupem v�ech vygenerovan�ch vstup� do motion modelu
		vector<double> d = vector<double>(inputCount);

		for (size_t i = 0; i < inputs.size(); i++)
		{
			auto input = inputs[i];
			auto tempState = carLikeMotionModel(nearState, input);	//this method changes near_node
			tempStates[i] = tempState;
			//Distance to s_rand when using different inputs
			d[i] = 0;
			for (auto uav : tempState->getUavsForRRT())
			{
				d[i] += randomState[*uav.get()]->getDistance(uav->getPointParticle()->getLocation());
			}
		}
		//d je pole vzd�lenost� mezi randomState a tempStaty. Vyb�r� se ten nejbli��� validn� tempState


		//TODO: opravit i p�i zm�n� po�tu vstup�, proto�e u tempState je nov� po�et vstup�, ale u nearState je star�, men�� po�et vstup�


		//TODO: mo�n� si sesortovat vzd�lenosti jednou bokem a pak p�istupovat k sesortovan�m vzd�lenostem, abych nemusel po��d hledat minValue. Zjistit hodnoty m, pokud je m vy�� ne� log(inputCount), vyplat� se sortovat
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
					break;	//v�jimka se nem� vyhazovat
				}

				//todo: pokud mo�no, refactorovat, a� nemus�m minimum hledat ru�n�
				int index = 0;	//kl�� nejmen�� hodnoty vzd�lenosti v poli d
				double minValue = DBL_MAX;
				for (size_t i = 0; i < d.size(); i++)
				{
					if (d[i] < minValue)
					{
						minValue = d[i];
						index = i;
					}
				}
				auto tempState = tempStates[index];	//temp state s nejmen�� vzd�lenost� od n�hodn� vybran�ho bodu z RRT-Path

				if (!collisionDetector->isStateValid(nearState, tempState, map))
				{
					d[index] = DBL_MAX; //jde o to vy�adit tuto hodnotu z hled�n� minima
					if (debug)
					{
						logger->logText("state with with index " + to_string(index) + " is not valid");
					}
					continue;
				}

				if (nearState->used_inputs.size() != inputCount)	//nastane ve chv�li, kdy je zapnuto smartZeroStepEnabling, UAV se p�ibl�� k p�ek�ce a zm�n� se po�et vstup�, je zapot�eb� upravit d�lku pole
				{
					nearState->used_inputs = vector<bool>(inputCount);		//zat�m je zde velikost natvrdo
					fill(nearState->used_inputs.begin(), nearState->used_inputs.end(), false);	//nastav�m false, snad to nebude vadit, kus stromu se projde znovu

				}

				if (nearState->used_inputs[index])	//used_inputs se pou��v� k tomu, jak� vstupy do motion modelu lze pou��t, abychom se pohnuli z nearState n�kam jinam
				{
					d[index] = DBL_MAX; //jde o to vy�adit tuto hodnotu z hled�n� minima
					if (debug)
					{
						logger->logText("input with index " + to_string(index) + " is used");
					}
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
			//todo: rozhodnout, zda to tady chci, nebo zda bude natvrdo zapnut� relativn� lokalizace
			//v�bec nech�pu, co tady d�l� m. Je �pln� out odf scope, z�st�v� na posledn� hodnot� for cyklu.
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
			for (auto node : guiding_path->getNodes())	//todo: mo�n� to p�ed�lat a iterovat obr�cen�, abych jel odzadu a ud�lal break, kdy� naraz�m na currentPoint u uav, abych nemusel kontrolovat isFirstCloserToEnd
			{
				for (auto uav : state->getUavsForRRT())
				{
					bool reached = false;
					if (uav->getPointParticle()->getLocation()->getDistanceSquared(node->getPoint()) < pow(guiding_near_dist, 2))
					{
						reached = true;
						//Narrow passage detection
						detect_narrow_passage(node);
					}
					auto uavCurrentPoint = uav->getCurrentGuidingPathPositions()->get(guiding_path);
					if (reached && guiding_path->hasNext(uavCurrentPoint) && guiding_path->isFirstCloserOrSameToEnd(node, uavCurrentPoint)) //o�et�en�, aby se UAV nevracela, a by nep�etekl index u guidingPath
					{
						uav->getCurrentGuidingPathPositions()->set(guiding_path, guiding_path->getNext(uavCurrentPoint));
						break;
					}
				}
			}
		}
	}

	void Core::check_near_goal(vector<shared_ptr<UavForRRT>> uavs, shared_ptr<Map> map)
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
			for (auto uav : uavs)	//aby se pak uav rozmis�ovala v r�mci cel� plochy, kter� pokr�v� c�le
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
		//todo: vymyslet, jak to ud�lat, abych odsud nastavil parametry, ale nesahal na celou konfiguraci. ud�lat asi n�jak� command na d�len� np_divisorem

//		global params defaults count
		if (node->getCost() > 1) {	//cost > 1 maj� p�ek�ky a nody soused�c� s p�ek�kami
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
	shared_ptr<LinkedState> Core::carLikeMotionModel(shared_ptr<LinkedState> state, unordered_map<UavForRRT, CarLikeControl, UavHasher> inputs)
	{
		auto newNode = stateFactory->createState(*state.get());	//copy constructor is called, makes deep copy
		// Simulation length
		double timeStep = configuration->getTimeStep();
				
		//main simulation loop
		//todo: v�ude, kde pou��v�m push_back se pod�vat, zda by ne�lo na za��tku naalokovat pole, aby se nemusela dynamicky m�nit velikost

		for (auto uav : newNode->getUavsForRRT())
		{
			motionModel->calculateState(uav, inputs[*uav.get()]);
			uav->setPreviousInput(inputs[*uav.get()]);
		}

		newNode->incrementTime(timeStep);

		return newNode;
	}

	double Core::getDistanceOfNewNodes(shared_ptr<Node> node)
	{
		double base = configuration->getDistanceOfNewNodes();
		if (configuration->isSlowerMotionNearObstacles())
		{
			base *= log(node->getDistanceToObstacle());	//�ist� heuristicky vymy�len� funkce, aby to nezrychlovalo moc, kdy� to bude d�l
		}
		return base;
	}

	Point Core::roundToNodeCoords(Point point)
	{
		//"zaokrouhl�" bod na st�ed node
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
		auto placementMethod = configuration->getPlacementMethod();
		if (allowSwarmSplitting || placementMethod == PlacementMethod::Chain)		//pro ��ely chainu rozd�l�m UAV na 2 skupiny, ka�d� m��� do jednoho c�le, a �et�z se neroztrhne jen d�ky relativn� lokalizaci
		{
			int number_of_uavs = map->getUavsStart().size();

			//rozd�lit kvadrokopt�ry na skupinky. Jedna skupina pro ka�d� AoI. Rozd�lit skupiny podle plochy, kterou jednotkliv� AoI zab�raj�
			//tohle je pro groups. 
			valarray<double> ratios = valarray<double>(guiding_paths.size()); //pom�ry jednotliv�ch ploch ku celkov� plo�e. Dlouh� jako po�et c�l�, tedy po�et guiding paths
			double totalVolume = 0;
			for (size_t i = 0; i < map->getGoals().size(); i++)
			{
				double volume = map->getGoals()[i]->getRectangle()->getVolume();
				ratios[i] = volume;
				totalVolume += volume;
			}

			ratios /= totalVolume;	//valarray umo��uje vektorov� operace, ka�d� prvek je v rozsahu od 0 do 1


									//p�erozd�lov�n� kvadrokopt�r podle pom�ru inspirov�no t�mto https://github.com/sebastianbergmann/money/blob/master/src/Money.php#L261

			int uavsInGroups = 0;	//po��t�, kolik uav je rozvr�en�ch do skupin, kv�li zaokrouhlov�n�
			for (size_t i = 0; i < uavGroups.size(); i++)
			{
				int uavsCountInGroup = floor(number_of_uavs * ratios[i]);	//round down
				auto uavs = vector<shared_ptr<UavForRRT>>(uavsCountInGroup);
				for (size_t j = 0; j < uavsCountInGroup; j++)
				{
					uavs[j] = state->getUavsForRRT()[uavsInGroups + j];	//todo: tuhle ��st asi zrefaktorovat. A n�kde m�t objekty reprezentuj�c� uav, s jeho polohou, apod.
				}
				uavGroups[i] = make_shared<UavGroup>(uavs, guiding_paths[i]);
				uavsInGroups += uavsCountInGroup;
			}
			//rozh�zet do skupin nep�i�azen� uav, kter� zbyla kv�li zaokrouhlov�n� dol�
			int remaining = map->getUavsStart().size() - uavsInGroups;
			for (size_t i = 0; i < remaining; i++)	//zbytku je v�dycky stejn� nebo m�n� ne� po�tu skupin
			{
				uavGroups[i]->addUav(state->getUavsForRRT()[uavsInGroups + i]);
			}
		}
		else
		{
			uavGroups[0] = make_shared<UavGroup>(state->getUavsForRRT(), guiding_paths[0]);	//v�m, �e p�i t�to konfiguraci allowSwarmSplitting je pouze 1 guidingPath, v�echna uav jsou v 1 skupin�
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

	void Core::checkSmartZeroStepEnabling(shared_ptr<LinkedState> state, shared_ptr<Map> map)
	{
		if (!configuration->getSmartZeroStepEnabling())
		{
			return;
		}

		int minimalDistance = configuration->getSmartZeroStepEnablingDistance();
		bool isUAVtooNearToObstacle = false;
		for (auto uav : state->getUavsForRRT())
		{
			for (auto obstacle : map->getObstacles())
			{
				if (obstacle->rectangle->getDistance(uav->getPointParticle()->getLocation()) < minimalDistance)
				{
					configuration->setZeroStepEnabled(true);
					return;
				}
			}
		}
	}

}
