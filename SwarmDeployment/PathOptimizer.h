#pragma once
#include "State.h"
#include "DistanceResolver.h"

namespace App
{

	class PathOptimizer
	{
	public:
		PathOptimizer(shared_ptr<DistanceResolver> distanceResolver);
		virtual ~PathOptimizer();
		vector<shared_ptr<State>> optimizePath(vector<shared_ptr<State>> path);

	protected:
		shared_ptr<DistanceResolver> distanceResolver;
	};


}
