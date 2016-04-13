#pragma once
#include "State.h"
#include "DistanceResolver.h"
#include "CollisionDetector.h"
#include "Dubins/geom/geom.h"
#include "LoggerInterface.h"
#include "MotionModel.h"
#include "Persister.h"
#include "Resampler.h"

namespace App
{
	enum class DubinsPart{Part1, Part2, Part3, None};
	enum class DubinsManeuver{Right, Straight, Left, None};

	class PathOptimizer
	{
	public:
		PathOptimizer(shared_ptr<DistanceResolver> distanceResolver, shared_ptr<Configuration> configuration, 
			shared_ptr<MotionModel> motionModel, shared_ptr<CollisionDetector> collisionDetector, 
			shared_ptr<LoggerInterface> logger, shared_ptr<Persister> persister, shared_ptr<Resampler> resampler);
		virtual ~PathOptimizer();
		vector<shared_ptr<State>> optimizePathByDubins(vector<shared_ptr<State>> path, shared_ptr<Map> map);
		pair<vector<shared_ptr<State>>, bool> optimizePathPart(int startIndex, int endIndex, shared_ptr<Map> map, vector<shared_ptr<State>> path);
		virtual void setLogger(const shared_ptr<LoggerInterface> logger_interface);
		vector<shared_ptr<State>> removeDuplicitStates(vector<shared_ptr<State>> path);

	protected:
		shared_ptr<DistanceResolver> distanceResolver;
		shared_ptr<Configuration> configuration;
		shared_ptr<MotionModel> motionModel;
		shared_ptr<CollisionDetector> collisionDetector;
		shared_ptr<LoggerInterface> logger;
		shared_ptr<Persister> persister;
		shared_ptr<Resampler> resampler;
		//pøedávám indexy, protože potøebuji iterovat od endu do konce pole
	};


}
