#pragma once
#include "LoggerInterface.h"
#include "memory"

using namespace std;

namespace App
{

	class Configuration;

	class Core : public enable_shared_from_this<Core>
	{
	
	public:
		explicit Core(shared_ptr<Configuration> configuration);
		virtual ~Core();
		void run();
		void setLogger(shared_ptr<LoggerInterface> logger);
		void logConfigurationChange();
		void rrtPath(vector<shared_ptr<Path>> paths, shared_ptr<Configuration> configuration);

	protected:
		shared_ptr<LoggerInterface> logger;
		vector<shared_ptr<Map>> maps;
		shared_ptr<Configuration> configuration;
		vector<shared_ptr<Point>> random_state_guided(vector<shared_ptr<Path>> guiding_paths, vector<vector<int>> current_index, vector<bool> goals_reached);
		shared_ptr<Node> nearest_neighbor(vector<shared_ptr<Point>> s_rand, vector<shared_ptr<Node>> nodes, int count);
		vector<shared_ptr<Node>> select_input(vector<shared_ptr<Point>> s_rand, shared_ptr<Node> near_node); // returns [near_node, new_node]
		void check_expandability();
	};

}

