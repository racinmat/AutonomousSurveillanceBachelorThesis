#include "PathOptimizer.h"
#include "Random.h"

namespace App
{


	PathOptimizer::PathOptimizer()
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
			//sem pøijde dubbins
			auto randomFirst = Random::element(path);
		}
		return path;
	}


}
