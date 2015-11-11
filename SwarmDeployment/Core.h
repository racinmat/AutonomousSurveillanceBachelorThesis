#pragma once
#include "LoggerInterface.h"
#include "memory"
#include "State.h"

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
		void rrtPath(vector<shared_ptr<Path>> paths, shared_ptr<Configuration> configuration, shared_ptr<Map> map);

	protected:
		shared_ptr<LoggerInterface> logger;
		vector<shared_ptr<Map>> maps;
		shared_ptr<Configuration> configuration;
		vector<shared_ptr<Point>> random_state_guided(vector<shared_ptr<Path>> guiding_paths, vector<vector<int>> current_index, vector<bool> goals_reached, shared_ptr<Map> map);
		shared_ptr<State> nearest_neighbor(vector<shared_ptr<Point>> s_rand, vector<shared_ptr<State>> nodes, int count);
		vector<shared_ptr<State>> select_input(vector<shared_ptr<Point>> s_rand, shared_ptr<State> near_node); // returns [near_node, new_node]
		int check_expandability(vector<shared_ptr<State>> nodes);
		vector<vector<int>> guiding_point_reached(shared_ptr<State> node, vector<shared_ptr<Path>> guiding_paths, vector<vector<int>> current_index, int guiding_near_dist);
		vector<int> check_near_goal(vector<shared_ptr<PointParticle>> uavs, shared_ptr<Map> map); // vrací pole dlouhé tak, jako je poèet UAV. pro každé UAv se tak uloží do tohoto pole èíslo podle toho, v kolikátém cíli UAV je. Parametr je pole, kde jsou polohy UAV
		void detect_narrow_passage(shared_ptr<Node> node);
		shared_ptr<Point> random_state_goal(shared_ptr<Goal> goal);
		shared_ptr<Point> random_state(shared_ptr<Rectangle> rectangle);
		shared_ptr<Point> random_state(int x1, int y1, int x2, int y2);
		bool check_inside_obstacle(shared_ptr<Point> point);
	};

}
