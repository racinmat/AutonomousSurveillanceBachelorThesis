#include "AoICoverageResolver.h"
#include <algorithm>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "Configuration.h"
#include <boost/spirit/home/karma.hpp>

using namespace boost::spirit::karma;

namespace App
{

	AoICoverageResolver::AoICoverageResolver(shared_ptr<Configuration> configuration) : configuration(configuration)
	{
	}

	AoICoverageResolver::~AoICoverageResolver()
	{
	}

	shared_ptr<LinkedState> AoICoverageResolver::get_best_fitness(vector<shared_ptr<LinkedState>> final_nodes, shared_ptr<Map> map)
	{
		initializeMapMatrix(map);

		auto finalStatesFitness = unordered_map<shared_ptr<LinkedState>, double>();
		for (auto finalState : final_nodes)
		{
			finalStatesFitness[finalState] = fitness_function(finalState, map);
		}

		//finding solution with best fitness
		pair<shared_ptr<LinkedState>, double> min = *min_element(finalStatesFitness.begin(), finalStatesFitness.end(),
			[](pair<shared_ptr<LinkedState>, double> a, pair<shared_ptr<LinkedState>, double> b) {return a.second < b.second; }
		);
		return min.first;
	}

	double AoICoverageResolver::fitness_function(shared_ptr<LinkedState> final_node, shared_ptr<Map> map)
	{
		ublas::matrix<double> currentGoalMatrix = ublas::matrix<double>(goalMatrix);	//copy cnstructor, so the original goal matrix does not change

		int elementSize = configuration->getGoalElementSize();
		int worldHeight = configuration->getWorldHeight();
		int worldWidth = configuration->getWorldWidth();
		double uavCameraX = configuration->getUavCameraX();
		double uavCameraY = configuration->getUavCameraY();
		double halfCameraX = floor(uavCameraX / 2);
		double halfCameraY = floor(uavCameraY / 2);
		double uavInitValue = 0.5;

		//do matice (vektoru vektorù) si budu ukládat hodnoty, jak uav vidí dané místo. matice je cíl diskretizovaný stejnì jako pøi a star hledání.
		//prázdný cíl má hodnotu 100. pokud cíl vidí UAV, vydìlí se hodnota dvìma. Nejmenší souèet je nejlepší.
		//matice UAV má všude 1, jen tam, kam vidí UAV, je 0.5. Pak se prvek po prvku vynásobí s maticí cílù. tím se vydìlí dvìma to, co vidí UAV a zbytek je nedotèen.

		//inicializace matice cílù. Všechny cíle jsou v jedné matici
		int rowCount = floor(worldHeight / elementSize);
		int columnCount = floor(worldWidth / elementSize);

		//inicializace matic UAV
		auto uavMatrixes = unordered_map<UavForRRT, ublas::matrix<double>, UavHasher>();
		for (auto uav : final_node->getUavsForRRT())
		{
			uavMatrixes[*uav.get()] = ublas::matrix<double>(rowCount, columnCount, 1);//initializes matrix with 1
		}
		for (auto uavMatrix : uavMatrixes)
		{
			auto uav = uavMatrix.first;
			auto matrix = uavMatrix.second;

			//filling goal in matrix with initial value
			auto loc = uav.getPointParticle()->getLocation();
			int minX = max<int>(floor((loc->getX() - halfCameraX) / elementSize), 0);
			int maxX = min<int>(floor((loc->getX() + halfCameraX) / elementSize), rowCount - 1);
			int minY = max<int>(floor((loc->getY() - halfCameraY) / elementSize), 0);
			int maxY = min<int>(floor((loc->getY() + halfCameraY) / elementSize), columnCount - 1);
			ublas::subrange(matrix, minX, maxX, minY, maxY) = ublas::matrix<double>(maxX - minX, maxY - minY, uavInitValue);
			uavMatrixes[uav] = matrix;	//for some weird reason, refence is not working, so I have to reassign the value
		}

		//vytvoøení prùniku nenulových hodnot a úprava hodnot matice mapy
		for (auto uavMatrix : uavMatrixes)
		{
			auto uav = uavMatrix.first;
			auto matrix = uavMatrix.second;

			currentGoalMatrix = element_prod(currentGoalMatrix, matrix);
		}

		std::ofstream("aoiMatrixes/node_" + to_string(final_node->getIndex()) + "_matrix.txt") << format_delimited(columns(currentGoalMatrix.size2())[auto_], '\t', currentGoalMatrix.data()) << endl;

		return sum(prod(ublas::scalar_vector<double>(currentGoalMatrix.size1()), currentGoalMatrix));	//sum of whole matrix, for weird reason, I must multiply here (prod), fuck you C++ http://stackoverflow.com/questions/24398059/how-do-i-sum-all-elements-in-a-ublas-matrix
	}

	void AoICoverageResolver::initializeMapMatrix(shared_ptr<Map> map)
	{
		int elementSize = configuration->getGoalElementSize();
		int worldHeight = configuration->getWorldHeight();
		int worldWidth = configuration->getWorldWidth();
		int rowCount = floor(worldHeight / elementSize);
		int columnCount = floor(worldWidth / elementSize);
		double notAoIInitialValue = 0;	//zero, because zero does not get any lower when divided
		double initialValue = 100;

		goalMatrix = ublas::matrix<double>(rowCount, columnCount, notAoIInitialValue);		//initializes matrix with really big value
		for (auto goal : map->getGoals())
		{
			//filling goal in matrix with initial value
			auto rect = goal->getRectangle();
			auto minX = max<double>(floor(rect->getX() / elementSize), 0);
			auto maxX = min<double>(floor(rect->getX() + rect->getWidth()) / elementSize, rowCount - 1);
			auto minY = max<double>(floor(rect->getY() / elementSize), 0);
			auto maxY = min<double>(floor(rect->getY() + rect->getHeight()) / elementSize, columnCount - 1);
			ublas::subrange(goalMatrix, minX, maxX, minY, maxY) = ublas::matrix<double>(maxX - minX, maxY - minY, initialValue);
		}
	}
}