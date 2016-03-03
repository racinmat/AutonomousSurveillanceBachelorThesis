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
		double getDistance(shared_ptr<StateInterface> first, unordered_map<UavInterface, shared_ptr<Point>, UavHasher> second);
		double getDistance(shared_ptr<LinkedState> first, unordered_map<UavForRRT, shared_ptr<Point>, UavHasher> second);
		double getDistance(shared_ptr<StateInterface> first, shared_ptr<StateInterface> second);
		double getDistance(shared_ptr<StateInterface> first, shared_ptr<StateInterface> second, shared_ptr<UavInterface> uav);
		double getLengthOfPath(shared_ptr<LinkedState> start, shared_ptr<LinkedState> end);
		double getLengthOfPath(vector<shared_ptr<State>> path);	//nemùžu mít vector<shared_ptr<interface>>, protože C++ je kterénské http://stackoverflow.com/questions/14992961/how-to-pass-a-vector-of-a-child-class-in-to-a-function-expecting-a-vector-of-par fuck you, C++
		double getLengthOfPath(vector<shared_ptr<LinkedState>> path);
		double getLengthOfPath(vector<shared_ptr<State>> path, shared_ptr<Uav> uav);

	protected:
		shared_ptr<Configuration> configuration;
	};


}
