#pragma once
#include "LoggerInterface.h"
#include "memory"
#include "State.h"
#include <fstream>
#include "StateFactory.h"
#include "Uav.h"

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


		vector<shared_ptr<Point>> random_state_guided(vector<shared_ptr<Path>> guiding_paths, shared_ptr<Map> map, shared_ptr<State> state);
		shared_ptr<State> nearest_neighbor(vector<shared_ptr<Point>> s_rand, vector<shared_ptr<State>> nodes, int count);
		shared_ptr<State> select_input(vector<shared_ptr<Point>> s_rand, shared_ptr<State> near_node, shared_ptr<Map> map); // returns [near_node, new_node]
		int check_expandability(vector<shared_ptr<State>> nodes);
		void guiding_point_reached(shared_ptr<State> node, vector<shared_ptr<Path>> guiding_paths, int guiding_near_dist);
		void check_near_goal(vector<shared_ptr<Uav>> uavs, shared_ptr<Map> map); // vrací pole dlouhé tak, jako je poèet UAV. pro každé UAv se tak uloží do tohoto pole èíslo podle toho, v kolikátém cíli UAV je. Parametr je pole, kde jsou polohy UAV
		void detect_narrow_passage(shared_ptr<Node> node);
		shared_ptr<Point> random_state_goal(shared_ptr<Goal> goal, shared_ptr<Map> map);
		shared_ptr<Point> random_state(shared_ptr<Rectangle> rectangle, shared_ptr<Map> map);
		shared_ptr<Point> random_state(int x1, int y1, int x2, int y2, shared_ptr<Map> map);
		bool check_inside_obstacle(shared_ptr<Point> point, shared_ptr<Map> map);
		shared_ptr<Point> random_state_polar(shared_ptr<Point> center, shared_ptr<Map> map, double radius_min, double radius_max);
		bool check_world_bounds(shared_ptr<Point> point, int worldWidth, int worldHeight);
		bool check_world_bounds(vector<shared_ptr<Uav>> points, int worldWidth, int worldHeight);
		template<typename T> vector<vector<T>> generateNTuplet(vector<T> usedChars, int tupletClass);
		shared_ptr<State> car_like_motion_model(shared_ptr<State> node, vector<shared_ptr<Point>> inputs);
		bool check_localization_sep(shared_ptr<State> node);
		bool trajectory_intersection(shared_ptr<State> near_node, shared_ptr<State> tmp_node);
		shared_ptr<State> check_obstacle_vcollide_single(shared_ptr<State> near_node, vector<vector<shared_ptr<Point>>> translation, int index, shared_ptr<Map> map);
		bool line_segments_intersection(shared_ptr<Point> p1, shared_ptr<Point> p2, shared_ptr<Point> p3, shared_ptr<Point> p4);
		bool line_point_intersection(shared_ptr<Point> q, shared_ptr<Point> p1, shared_ptr<Point> p2);
		shared_ptr<Point> line_line_intersection(shared_ptr<Point> p1, shared_ptr<Point> p2, shared_ptr<Point> p3, shared_ptr<Point> p4);


	protected:
		shared_ptr<LoggerInterface> logger;
		vector<shared_ptr<Map>> maps;
		shared_ptr<Configuration> configuration;
		shared_ptr<StateFactory> stateFactory;


		void testGui();

		ofstream file;	//todo: udìlat nìjaký poøádný logger, tohle je doèasné na rychlé logování
	};

	class rrtPathError : public std::runtime_error
	{
	public:
		rrtPathError(const string& _Message) : std::runtime_error(_Message) { }
		rrtPathError(const char* _Message) : std::runtime_error(_Message) { }
	};
}

