#pragma once
#include "State.h"
#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric;

namespace App
{

	class AoICoverageResolver
	{
	public:
		AoICoverageResolver();
		virtual ~AoICoverageResolver();
		shared_ptr<LinkedState> get_best_fitness(vector<shared_ptr<LinkedState>> final_nodes, shared_ptr<Map> map, int elementSize, int worldWidth, int worldHeight);

	protected:
		double fitness_function(shared_ptr<LinkedState> final_node, shared_ptr<Map> map, int elementSize, int worldWidth, int worldHeight);
		ublas::matrix<double> goalMatrix; //jako property kvùli cacheování
		bool goalMatrixInitialized;
	};

}
