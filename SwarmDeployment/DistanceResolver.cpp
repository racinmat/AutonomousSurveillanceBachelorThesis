#include "DistanceResolver.h"
#include "Configuration.h"

namespace App
{

	DistanceResolver::DistanceResolver(shared_ptr<Configuration> configuration) : 
		configuration(configuration)
	{
	}

	DistanceResolver::~DistanceResolver()
	{
	}

	double DistanceResolver::getDistance(shared_ptr<StateInterface> first, unordered_map<UavForRRT, shared_ptr<Point>, UavHasher> second)
	{
		NNMethod nn_method = configuration->getNearestNeighborMethod();
		double totalDistance = 0;
		unordered_map<UavForRRT, double, UavHasher> distances = unordered_map<UavForRRT, double, UavHasher>(first->getUavs().size());

		for (auto uav : first->getUavs())
		{
			auto anotherState = second[*uav.get()];
			distances[*uav.get()] = uav->getPointParticle()->getLocation()->getDistance(anotherState);
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

	double DistanceResolver::getDistance(shared_ptr<StateInterface> first, shared_ptr<StateInterface> second)
	{
		//optimization for NNMethod::Total, is 7.5 times faster than general method.
		if (configuration->getNearestNeighborMethod() == NNMethod::Total)
		{
			double distance = 0;
			for (auto uav : first->getUavs())
			{
				auto uavInSecondState = second->getUav(uav)->getPointParticle()->getLocation();
				distance += uav->getPointParticle()->getLocation()->getDistance(uavInSecondState);
			}
			return distance;
		}

		auto secondMap = unordered_map<UavForRRT, shared_ptr<Point>, UavHasher>();
		for (auto uav : second->getUavs())
		{
			secondMap[*uav.get()] = uav->getPointParticle()->getLocation();
		}
		return getDistance(first, secondMap);
	}

	double DistanceResolver::getDistance(shared_ptr<StateInterface> first, shared_ptr<StateInterface> second, shared_ptr<UavForRRT> uav)
	{
		return first->getUav(uav)->getPointParticle()->getLocation()->getDistance(second->getUav(uav)->getPointParticle()->getLocation());
	}

	double DistanceResolver::getLengthOfPath(shared_ptr<LinkedState> start, shared_ptr<LinkedState> end)
	{
		double length = 0;
		for (auto i = end; i->getTime() != start->getTime(); i = i->getPrevious())	//i jede od konce po prvek, jehož pøedchùdce je zaèátek
		{
			auto previous = i->getPrevious();
			length += getDistance(previous, i);	//todo: vymyslet, jak najít vždálenost poèítanou po kružnici, a ne po výsledných pøímkách
		}
		return length;
	}

	double DistanceResolver::getLengthOfPath(vector<shared_ptr<State>> path)
	{
		double length = 0;
		for (size_t i = 1; i < path.size(); i++)
		{
			length += getDistance(path[i - 1], path[i]);
		}
		return length;
	}

	double DistanceResolver::getLengthOfPath(vector<shared_ptr<LinkedState>> path)
	{
		double length = 0;
		for (size_t i = 1; i < path.size(); i++)
		{
			length += getDistance(path[i - 1], path[i]);
		}
		return length;
	}


	double DistanceResolver::getLengthOfPath(vector<shared_ptr<State>> path, shared_ptr<UavForRRT> uav)
	{
		double length = 0;
		for (size_t i = 1; i < path.size(); i++)
		{
			length += getDistance(path[i - 1], path[i], uav);	//todo: vymyslet, jak najít vždálenost poèítanou po kružnici, a ne po výsledných pøímkách
		}
		return length;
	}

}
