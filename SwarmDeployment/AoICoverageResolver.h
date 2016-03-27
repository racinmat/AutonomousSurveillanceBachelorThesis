#pragma once
#include "State.h"
#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric;

namespace App
{
	class Configuration;

	class AoICoverageResolver
	{
	public:
		AoICoverageResolver(shared_ptr<Configuration> configuration);
		virtual ~AoICoverageResolver();
		shared_ptr<LinkedState> get_best_fitness(vector<shared_ptr<LinkedState>> final_nodes, shared_ptr<Map> map);

	protected:
		double fitness_function(shared_ptr<LinkedState> final_node, shared_ptr<Map> map);
		ublas::matrix<double> goalMatrix; //jako property kvùli cacheování
		bool goalMatrixInitialized;
		shared_ptr<Configuration> configuration;
	};

}
