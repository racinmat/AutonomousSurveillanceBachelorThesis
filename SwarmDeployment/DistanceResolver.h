#pragma once
#include <memory>
#include "Configuration.h"

namespace App
{

	class DistanceResolver
	{
	public:
		DistanceResolver(shared_ptr<Configuration> configuration);
		virtual ~DistanceResolver();
		double getDistance(shared_ptr<State> first, unordered_map<Uav, shared_ptr<Point>, UavHasher> second);
		double getDistance(shared_ptr<State> first, shared_ptr<State> second);

	protected:
		shared_ptr<Configuration> configuration;
	};


}
