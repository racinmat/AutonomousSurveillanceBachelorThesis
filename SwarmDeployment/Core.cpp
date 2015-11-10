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

//		rrtPath(paths, configuration);
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

	void Core::rrtPath(vector<shared_ptr<Path>> guiding_paths, shared_ptr<Configuration> configuration)
	{
		int uavCount = configuration->getUavCount();
		int rrt_min_nodes = 1;
		int rrt_max_nodes = 20000;
		bool stop = false;
		int number_of_solutions = 10000;
		int near_count = 1000;
		int number_of_inputs = 12;	//todo: spoèítat z dalších hodnot z konfigurace, je to v Node.h

//		global params number_of_uavs empty_node nodes goals goal_map stop output

		cout << "Starting RRT-path...";

		// celková dráha všech cest dohromady, poèet nodes ve všech cestách
		// dohromady, evidentnì se ta promìnná nikde nepoužívá
		int gp_length = 0;
		for (int m = 0; m < guiding_paths.size(); m++)
		{
			gp_length += guiding_paths[m]->getSize();
		}

		vector<shared_ptr<Node>> nodes = vector<shared_ptr<Node>>(); //todo: zjistit, na jaké hodnoty to inicializovat

		auto current_index = vector<vector<int>>(uavCount);		// matice s délkami cest pro jednotlivá UAV.sloupec je cesta, øádek je UAV

		for (int i = 0; i < current_index.size(); i++)
		{
			current_index[i] = vector<int>(guiding_paths.size());
			for (int j = 0; j < guiding_paths.size(); j++)
			{
				current_index[i][j] = guiding_paths[i]->getSize();
			}
		}

		auto final_nodes = vector<shared_ptr<Node>>(rrt_max_nodes);

		auto goals_reached = vector<bool>(uavCount);
		for (auto goal_reached : goals_reached)
		{
			goal_reached = false;
		}


		int nodes_count = rrt_max_nodes;


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
			vector<shared_ptr<Point>> s_rand = random_state_guided(guiding_paths, current_index, goals_reached); // vrátí pole náhodných bodù, jeden pro každou kvadrokoptéru

			//Finding appropriate nearest neighbor
			int k = 0;	//poèítadlo uvíznutí
			bool near_found = false;

			//opakování, dokud nenajdu vyhovující øešení, poèítají se prùchody cyklem kvùli uvíznutí
			while (!near_found)
			{
				if (k > near_count)
				{
					i--;
					throw "Not possible to find near node suitable for expansion";
				}
				shared_ptr<Node> near_node = nearest_neighbor(s_rand, nodes, k);
				vector<shared_ptr<Node>> returnedNodes = select_input(s_rand, near_node);
				// Vypadá to, že near_node je ve funkci select_input zmìnìná kvùli kontrole pøekážek
				near_node = returnedNodes[0];
				shared_ptr<Node> new_node = returnedNodes[1];
				nodes[near_node->index] = near_node; // promìnná nodes je pole, kam se ukládá strom prohledávání u RRT - Path

				bool allInputsUsed = true;
				for (bool inputUsed : near_node->used_inputs)
				{
					allInputsUsed = allInputsUsed && inputUsed;
				}

				auto isNearUavPosition = false;
				for (auto uavPosition : near_node->uavs)
				{
					allInputsUsed = isNearUavPosition || (uavPosition.get() != nullptr);	//pozice je null, pokud se pro UAV nenašla vhodná další pozice
				}

				auto isNewUavPosition = false;
				for (auto uavPosition : new_node->uavs)
				{
					allInputsUsed = isNewUavPosition || (uavPosition.get() != nullptr);	//pozice je null, pokud se pro UAV nenašla vhodná další pozice
				}

				//poèítadlo uvíznutí. UAV uvízlo, pokud je tento if true
				if (allInputsUsed || !isNewUavPosition || !isNearUavPosition)
				{
					k++;
					check_expandability();
				} else
				{
					near_found = true;
				}
			}





//			if any(isnan(new_node.loc(1, :)))
//				check_expandability();
//		disp('NaN in new node');
//		final_nodes(m) = nodes(i - 1);
//		break
//			end
//
//			new_node.index = i;
//		new_node.time_added = toc(tstart);
//		if params.debug
//			fprintf('[debug] Added node index: %d\n', new_node.index);
//		end
//
//			if mod(i, 200) == 0
//				fprintf('RRT size: %d\n', i);
//		end
//
//			nodes(i) = new_node;
//		s = s + 1;
//
//		current_index = guiding_point_reached(new_node, guiding_paths, current_index); % zde se uloží do current_index, kolik nodes zbývá danému UAV do cíle
//			goal_reached = check_near_goal(new_node.loc);
//		if all(~isnan(goal_reached)) % pokud je nalezen cíl
//			output.goal_reached = goal_reached;
//		final_nodes(m) = new_node;
//		fprintf('%d viable paths found so far.\n', m);
//		part_covered(1, end + 1) = goal_map.get_area_covered(); %#ok
//			goal_map.reset();
//		m = m + 1;
//		end
//			output.distance_of_new_nodes(1, i) = params.distance_of_new_nodes;
//		%     dt = toc(tv);
//		%     output.time_to_find_new_node(1, i) = dt;
//		%     output.iterations(1, i) = i;
//		%     try
//			%         output.time_running(1, i) = output.time_running(1, i - 1) + dt;
//		%     catch
//			%         output.time_running(1, i) = dt;
//		%     end
//			%     if output.time_running(1, i) > params.max_iteration_time
//			%         final_nodes(m) = new_node;
//		%         return
//			%     end
//			%     output.goal_reached = goal_reached;
//
//		%Visualize
//			if params.visualize == true && mod(i, params.draw_freq) == 0
//				visualize_growth(near_node.loc, new_node.loc, s_rand);
//		end
//
		}
		
//			end
//			final_nodes(m) = nodes(i);
//		goal_reached = check_near_goal(new_node.loc);
//		output.goal_reached = goal_reached;
//		final_nodes = final_nodes(1:(find(isnan([final_nodes.index]), 1) - 1));
//		if any(isnan([nodes.index]))
//			nodes = nodes(1:(find(isnan([nodes.index]), 1) - 1));
//		end
//			output.nodes = nodes;
		cout << "RRT-Path finished";
	}

	vector<shared_ptr<Point>> Core::random_state_guided(vector<shared_ptr<Path>> guiding_paths, vector<vector<int>> current_index, vector<bool> goals_reached)
	{
		return vector<shared_ptr<Point>>();
	}

	shared_ptr<Node> Core::nearest_neighbor(vector<shared_ptr<Point>> s_rand, vector<shared_ptr<Node>> nodes, int count)
	{
		return make_shared<Node>(make_shared<Point>(0, 0), Grid::Free);
	}

	vector<shared_ptr<Node>> Core::select_input(vector<shared_ptr<Point>> s_rand, shared_ptr<Node> near_node)
	{
		return vector<shared_ptr<Node>>();
	}

	void Core::check_expandability()
	{
	}
}
