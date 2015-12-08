#pragma once
#include "LoggerInterface.h"
#include "memory"
#include "State.h"
#include <fstream>
#include "StateFactory.h"
#include "Uav.h"
#include <unordered_map>
#include "Output.h"
#include "NTupletGenerator.h"
#include "InputGenerator.h"
#include <map>
#include "UavGroup.h"
#include "AoICoverageResolver.h"
#include "DistanceResolver.h"
#include "PathOptimizer.h"
#include "CarLikeControl.h"
#include "CarLikeMotionModel.h"
#include "CollisionDetector.h"

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
		shared_ptr<Output> rrtPath(vector<shared_ptr<Path>> paths, shared_ptr<Configuration> configuration, shared_ptr<Map> map, vector<shared_ptr<Node>> mapNodes);


		unordered_map<Uav, shared_ptr<Point>, UavHasher> random_state_guided(vector<shared_ptr<Path>> guiding_paths, shared_ptr<Map> map, shared_ptr<LinkedState> state);
		shared_ptr<LinkedState> nearest_neighbor(unordered_map<Uav, shared_ptr<Point>, UavHasher> s_rand, vector<shared_ptr<LinkedState>> nodes, int count);
		shared_ptr<LinkedState> select_input(unordered_map<Uav, shared_ptr<Point>, UavHasher> s_rand, shared_ptr<LinkedState> near_node, shared_ptr<Map> map, std::map<string, shared_ptr<Node>> mapNodes);
		int check_expandability(vector<shared_ptr<LinkedState>> nodes);
		void guiding_point_reached(shared_ptr<LinkedState> node, vector<shared_ptr<Path>> guiding_paths, double guiding_near_dist);
		void check_near_goal(vector<shared_ptr<Uav>> uavs, shared_ptr<Map> map); // vrací pole dlouhé tak, jako je poèet UAV. pro každé UAv se tak uloží do tohoto pole èíslo podle toho, v kolikátém cíli UAV je. Parametr je pole, kde jsou polohy UAV
		void detect_narrow_passage(shared_ptr<Node> node);
		shared_ptr<Point> random_state_goal(shared_ptr<GoalInterface> goal, shared_ptr<Map> map);
		shared_ptr<Point> random_state(int x1, int y1, int x2, int y2, shared_ptr<Map> map);
		bool check_inside_obstacle(shared_ptr<Point> point, shared_ptr<Map> map);
		shared_ptr<Point> random_state_polar(shared_ptr<Point> center, shared_ptr<Map> map, double radius_min, double radius_max);
		shared_ptr<LinkedState> carLikeMotionModel(shared_ptr<LinkedState> state, unordered_map<Uav, shared_ptr<CarLikeControl>, UavHasher> inputs);
		double getDistanceOfNewNodes(shared_ptr<Node> node);
		Point roundToNodeCoords(Point point);	//zaokrouhlí bod na souøadnice støedu node, abych mohl vyhledávat efektivnì mezi nodami
		vector<shared_ptr<UavGroup>> splitUavsToGroups(vector<shared_ptr<Path>> guiding_paths, shared_ptr<Map> map, shared_ptr<LinkedState> state, bool allowSwarmSplitting);
		shared_ptr<LinkedState> get_closest_node_to_goal(vector<shared_ptr<LinkedState>> states, vector<shared_ptr<Path>> guiding_paths, shared_ptr<Map> map);
		void save_output();

	protected:
		shared_ptr<LoggerInterface> logger;
		vector<shared_ptr<Map>> maps;
		shared_ptr<Configuration> configuration;
		shared_ptr<StateFactory> stateFactory;
		shared_ptr<InputGenerator> inputGenerator;
		shared_ptr<AoICoverageResolver> coverageResolver;
		shared_ptr<DistanceResolver> distanceResolver;
		shared_ptr<PathOptimizer> pathOptimizer;
		shared_ptr<CarLikeMotionModel> motionModel;
		shared_ptr<CollisionDetector> collisionDetector;

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

