#pragma once
#include <memory>
#include "State.h"
#include "VCollide/Triangle3D.h"
#include "LoggerInterface.h"

using namespace std;

namespace App
{
	class Configuration;

	class CollisionDetector
	{
	public:
		CollisionDetector(shared_ptr<Configuration> configuration);
		virtual ~CollisionDetector();
		bool areTrajectoriesIntersecting(shared_ptr<StateInterface> start, shared_ptr<StateInterface> end);
		bool areLineSegmentsIntersecting(shared_ptr<Point> p1, shared_ptr<Point> p2, shared_ptr<Point> p3, shared_ptr<Point> p4);
		bool areLineSegmentsIntersecting(shared_ptr<UavForRRT> uav1start, shared_ptr<UavForRRT> uav1end, shared_ptr<UavForRRT> uav2start, shared_ptr<UavForRRT> uav2end);
		bool LinePointIntersection(shared_ptr<Point> q, shared_ptr<Point> p1, shared_ptr<Point> p2);
		shared_ptr<Point> lineLineIntersection(shared_ptr<Point> p1, shared_ptr<Point> p2, shared_ptr<Point> p3, shared_ptr<Point> p4);
		pair<shared_ptr<UavForRRT>, shared_ptr<UavForRRT>> getIntersectingUavs(shared_ptr<StateInterface> start, shared_ptr<StateInterface> end);
		bool checkRelativeLocalization(shared_ptr<StateInterface> node);
		bool insideWorldBounds(shared_ptr<Point> point, int worldWidth, int worldHeight);
		bool insideWorldBounds(vector<shared_ptr<UavForRRT>> points, int worldWidth, int worldHeight);
		bool isStateValid(shared_ptr<StateInterface> oldState, shared_ptr<StateInterface> newState, shared_ptr<Map> map);
		bool check_obstacle_vcollide_single(shared_ptr<StateInterface> new_node, shared_ptr<Map> map);
		bool checkObstaclesInTrajectories(shared_ptr<StateInterface> oldState, shared_ptr<StateInterface> newState, shared_ptr<Map> map);
		bool isInitialSwarmStateFeasible(shared_ptr<StateInterface> state);
		void setLogger(shared_ptr<LoggerInterface> logger);

	protected:
		shared_ptr<Configuration> configuration;
		shared_ptr<LoggerInterface> logger;
		bool collidesWithObstacles(Triangle3D triangle, shared_ptr<Map> map);
	};

}
