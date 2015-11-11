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
#include <memory>
#include <string>
#include "Output.h"
#include "State.h"
#include "Random.h"

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

		auto current_index = vector<vector<int>>(uavCount);		// matice s délkami cest pro jednotlivá UAV.sloupec je cesta, øádek je UAV

		for (int i = 0; i < current_index.size(); i++)
		{
			current_index[i] = vector<int>(guiding_paths.size());
			for (int j = 0; j < guiding_paths.size(); j++)
			{
				current_index[i][j] = guiding_paths[i]->getSize();
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
				shared_ptr<State> near_node = nearest_neighbor(s_rand, nodes, k);
				vector<shared_ptr<State>> returnedNodes = select_input(s_rand, near_node);
				// Vypadá to, že near_node je ve funkci select_input zmìnìná kvùli kontrole pøekážek
				near_node = returnedNodes[0];
				new_node = returnedNodes[1];
				nodes[near_node->index] = near_node; // promìnná nodes je pole, kam se ukládá strom prohledávání u RRT - Path. Nemìlo by být potøeba tohle pøiøazovat, protože tam je reference, ne hodnota

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
		}
		
		final_nodes[m] = nodes[i];
		goal_reached_by_all_uavs = check_near_goal(new_node->uavs, map);
		output->goal_reached = goal_reached_by_all_uavs;
//		final_nodes = final_nodes(1:(find(isnan([final_nodes->index]), 1) - 1)); //zjistit, co to dìlá a získat final nodes. a zjistit, co se má najít
//		if (any(isnan([nodes.index])))
//		{
//			nodes = nodes(1:(find(isnan([nodes.index]), 1) - 1));
//		}
		output->nodes = nodes;
		cout << "RRT-Path finished";
	}

	vector<shared_ptr<Point>> Core::random_state_guided(vector<shared_ptr<Path>> guiding_paths, vector<vector<int>> current_index, vector<bool> goals_reached, shared_ptr<Map> map)
	{
		double guided_sampling_prob = 1;
		int worldWidth = 1000;
		int worldHeight = 1000;
		int number_of_uavs = map->getUavsStart().size();

//		return vector<shared_ptr<Point>>();
		vector<double> propabilities = vector<double>(guiding_paths.size());
		
//		global number_of_uavs params

		int sum = 0; // sum = celková délka všech vedoucích cest

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
				if (goals_reached[i] > 0)
				{//todo: s tímhle nìco udìlat, a nepøistupovat k poli takhle teple pøes indexy
					s_rand[i] = random_state_goal(map->getGoals()[goals_reached[i]]);	//pokud je n-té UAV v cíli, vybere se náhodný bod z cílové plochy, kam UAV dorazilo
				} else
				{
					s_rand[i] = random_state(0, worldWidth, 0, worldHeight); // pokud n-té UAV není v cíli, vybere se náhodný bod z celé mapy
				}
			}
		} else
		{
			//implementovat, až zjistím, k èemu je ratio

//		        ratio = number_of_uavs/length(guiding_paths); % pomìr UAV k poètu vedoucích cest, dìlá bordel, když není integer. Zjistit, co je za èíslo.
//		        for m=1:length(guiding_paths)
//		            guiding_path = guiding_paths{m};
//		            best_reached = min(index((m-1)*ratio+1:(m-1)*ratio+ratio,m));
//		            
//		%             assignin('base', 'index', index((m-1)*ratio+1:(m-1)*ratio+ratio,m));
//		%             assignin('base', 'best_reached', best_reached);
//		            
//		            center = [guiding_path(best_reached).x; ...
//		                guiding_path(best_reached).y];
//		            for n=1:ratio
//		                if goal_reached(1,(m-1)*ratio+n) == m
//		                    s_rand(:,(m-1)*ratio+n) = random_state_goal(goal_reached(1,(m-1)*ratio+n));
//		                else    % náhodná pozice z okruhu okolo nejbližšího bodu nejbližší vedoucí cesty
//		                    while true
//		                        s_rand(:,(m-1)*ratio+n) = random_state_polar(center, 0, params.sampling_radius);
//		   %                     if ~check_inside_obstacle(s_rand(:,(m-1)*ratio+n))
//		                            break
//		    %s                    end
//		                    end
//		   %                 break
//		                end
//		            end
//		        end
			
		}
	}

	shared_ptr<State> Core::nearest_neighbor(vector<shared_ptr<Point>> s_rand, vector<shared_ptr<State>> nodes, int count)
	{
		return make_shared<State>();
	}

	vector<shared_ptr<State>> Core::select_input(vector<shared_ptr<Point>> s_rand, shared_ptr<State> near_node)
	{
		return vector<shared_ptr<State>>();
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

	shared_ptr<Point> Core::random_state_goal(shared_ptr<Goal> goal)
	{
		return random_state(goal->rectangle);
	}

	shared_ptr<Point> Core::random_state(shared_ptr<Rectangle> rectangle)
	{
		return random_state(rectangle->getX(), rectangle->getX() + rectangle->getWidth(), rectangle->getY(), rectangle->getY() + rectangle->getHeight());
	}

	shared_ptr<Point> Core::random_state(int x1, int y1, int x2, int y2)
	{
		shared_ptr<Point> random_state;
		do
		{
			int x = Random::inRange(x1, x2);
			int y = Random::inRange(y1, y2);
			random_state = make_shared<Point>(x, y);
		} while (check_inside_obstacle(random_state));
		return random_state;
	}

	//returns true if point is inside of obstacle
	bool Core::check_inside_obstacle(shared_ptr<Point> point)
	{
	}
}

//todo: zjistit, jak funguje ratio u metody 3 u rrt_path sampling. U 4 UAV, mapy 6 a metody vzorkování 3 vyletí error kvùli doublu, necelému èíslu, v promìnné ratio