#pragma once
#include "State.h"
#include "DistanceResolver.h"
#include "CarLikeMotionModel.h"
#include "CollisionDetector.h"
#include "Dubins/geom/geom.h"
#include "LoggerInterface.h"

namespace App
{
	enum class DubinsPart{Part1, Part2, Part3, None};
	enum class DubinsManeuver{Right, Straight, Left, None};

	class PathOptimizer
	{
	public:
		PathOptimizer(shared_ptr<DistanceResolver> distanceResolver, shared_ptr<Configuration> configuration, 
			shared_ptr<CarLikeMotionModel> motionModel, shared_ptr<CollisionDetector> collisionDetector, 
			shared_ptr<LoggerInterface> logger);
		virtual ~PathOptimizer();
		vector<shared_ptr<State>> optimizePath(vector<shared_ptr<State>> path, shared_ptr<Map> map);
		pair<vector<shared_ptr<State>>, bool> optimizePathPart(vector<shared_ptr<State>> pathPart, shared_ptr<Map> map);
		virtual void setLogger(const shared_ptr<LoggerInterface> logger_interface);

	protected:
		shared_ptr<DistanceResolver> distanceResolver;
		shared_ptr<Configuration> configuration;
		shared_ptr<CarLikeMotionModel> motionModel;
		shared_ptr<CollisionDetector> collisionDetector;
		shared_ptr<LoggerInterface> logger;
		void straightenCrossingTrajectories(shared_ptr<State> start, shared_ptr<State> end);
	};


}
