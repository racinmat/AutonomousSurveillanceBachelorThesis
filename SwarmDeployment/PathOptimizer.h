#pragma once
#include "State.h"
#include "DistanceResolver.h"
#include "CarLikeMotionModel.h"

namespace App
{

	class PathOptimizer
	{
	public:
		PathOptimizer(shared_ptr<DistanceResolver> distanceResolver, shared_ptr<Configuration> configuration, shared_ptr<CarLikeMotionModel> motionModel);
		virtual ~PathOptimizer();
		vector<shared_ptr<State>> optimizePath(vector<shared_ptr<State>> path);

	protected:
		shared_ptr<DistanceResolver> distanceResolver;
		shared_ptr<Configuration> configuration;
		shared_ptr<CarLikeMotionModel> motionModel;
	};


}
