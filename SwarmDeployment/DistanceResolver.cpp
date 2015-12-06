#include "DistanceResolver.h"


namespace App
{

	DistanceResolver::DistanceResolver(shared_ptr<Configuration> configuration) : 
		configuration(configuration)
	{
	}

	DistanceResolver::~DistanceResolver()
	{
	}

	double DistanceResolver::getDistance(shared_ptr<State> first, unordered_map<Uav, shared_ptr<Point>, UavHasher> second)
	{
		NNMethod nn_method = configuration->getNearestNeighborMethod();
		double totalDistance = 0;
		unordered_map<Uav, double, UavHasher> distances = unordered_map<Uav, double, UavHasher>(first->getUavs().size());

		for (auto uav : first->getUavs())
		{
			auto randomState = second[*uav.get()];
			distances[*uav.get()] = uav->getPointParticle()->getLocation()->getDistanceSquared(randomState);
		}

		switch (nn_method)
		{
		case NNMethod::Total:
			for (auto dist : distances)
			{
				auto distance = dist.second;
				totalDistance += distance;	//no function for sum, so I must do it by hand
			}
			break;
		case NNMethod::Max:
			totalDistance = (*max_element(distances.begin(), distances.end())).second;	//tohle vrací iterátor, který musím dereferencovat, abych získal èíslo. fuck you, C++
			break;
		case NNMethod::Min:
			totalDistance = (*min_element(distances.begin(), distances.end())).second;	//tohle vrací iterátor, který musím dereferencovat, abych získal èíslo. fuck you, C++
			break;
		}
		return totalDistance;
	}

	double DistanceResolver::getDistance(shared_ptr<State> first, shared_ptr<State> second)
	{
		auto secondMap = unordered_map<Uav, shared_ptr<Point>, UavHasher>();
		for (auto uav : second->getUavs())
		{
			secondMap[*uav.get()] = uav->getPointParticle()->getLocation();
		}
	}

}
