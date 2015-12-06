#include "PathOptimizer.h"
#include "Random.h"

namespace App
{


	PathOptimizer::PathOptimizer(shared_ptr<DistanceResolver> distanceResolver) : 
		distanceResolver(distanceResolver)
	{
	}


	PathOptimizer::~PathOptimizer()
	{
	}

	//optimalizuje cestu pomocí Dubinsových manévrù
	vector<shared_ptr<State>> PathOptimizer::optimizePath(vector<shared_ptr<State>> path)
	{
		int stopLimit = 100;	//kolikrát za sebou se nesmí aplikování Dubinse zlepšit trajektorie, aby se algoritmus zastavil
		int notImprovedCount = 0;

		while (notImprovedCount < stopLimit)
		{
			auto randomFirst = Random::element(path);
			auto randomSecond = Random::element(path);
			while (*randomFirst.get() == *randomSecond.get())
			{
				randomSecond = Random::element(path);
			}
			if (randomFirst->getIndex() > randomSecond->getIndex())
			{
				//swap
				auto temp = randomSecond;
				randomSecond = randomFirst;
				randomFirst = temp;
			}

			for (auto uav : randomSecond->getUavs())
			{

				double distance = 0;
				for (auto i = randomSecond; *i.get() != *randomFirst.get(); i = i->getPrevious())	//i jede od konce po prvek, jehož pøedchùdce je zaèátek
				{
					auto previous = i->getPrevious();
					distance += distanceResolver->getDistance(previous, i);
				}

				//sem pøijde dubins
				double newDistance = 0;

				if (newDistance < distance)
				{
					//todo: udìlat validace
					bool isNewPathValid = true;

					if (isNewPathValid)
					{

					}
					//zde se vyhodí delší èást cesty a místo ní se sem dá kratší èást cesty
				}
			}
		}
		return path;
	}


}
