#pragma once
#include <memory>
#include "State.h"

using namespace std;

namespace App
{
	class Configuration;

	class DistanceResolver
	{
	public:
		DistanceResolver(shared_ptr<Configuration> configuration);
		virtual ~DistanceResolver();
		double getDistance(shared_ptr<LinkedState> first, unordered_map<Uav, shared_ptr<Point>, UavHasher> second);
		double getDistance(shared_ptr<LinkedState> first, shared_ptr<LinkedState> second);
		double getDistance(shared_ptr<LinkedState> first, shared_ptr<LinkedState> second, shared_ptr<Uav> uav);
		double getLengthOfPath(shared_ptr<LinkedState> start, shared_ptr<LinkedState> end);
		double getLengthOfPath(vector<shared_ptr<LinkedState>> path);
		double getLengthOfPath(shared_ptr<LinkedState> start, shared_ptr<LinkedState> end, shared_ptr<Uav> uav);

	protected:
		shared_ptr<Configuration> configuration;
	};


}
