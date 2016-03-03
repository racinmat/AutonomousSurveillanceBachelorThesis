#include "AoICoverageResolver.h"
#include <algorithm>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

namespace App
{

	AoICoverageResolver::AoICoverageResolver()
	{
	}


	AoICoverageResolver::~AoICoverageResolver()
	{
	}

	shared_ptr<LinkedState> AoICoverageResolver::get_best_fitness(vector<shared_ptr<LinkedState>> final_nodes, shared_ptr<Map> map, int elementSize, int worldWidth, int worldHeight)
	{
		goalMatrixInitialized = false;	//invalidace cache pøed zaèátkem hledání

		auto finalStatesFitness = unordered_map<shared_ptr<LinkedState>, double>();
		for (auto finalState : final_nodes)
		{
			finalStatesFitness[finalState] = fitness_function(finalState, map, elementSize, worldWidth, worldHeight);
		}

		//finding solution with best fitness
		pair<shared_ptr<LinkedState>, double> min = *min_element(finalStatesFitness.begin(), finalStatesFitness.end(),
			[](pair<shared_ptr<LinkedState>, double> a, pair<shared_ptr<LinkedState>, double> b) {return a.second < b.second; }
		);
		return min.first;
	}

	double AoICoverageResolver::fitness_function(shared_ptr<LinkedState> final_node, shared_ptr<Map> map, int elementSize, int worldWidth, int worldHeight)
	{
		double initialValue = 100;
		double uavCameraX = (150 / elementSize);
		double uavCameraY = floor(100 / elementSize);
		double halfCameraX = floor(uavCameraX / 2);
		double halfCameraY = floor(uavCameraY / 2);
		double uavInitValue = 1 / 2;

		//do matice (vektoru vektorù) si budu ukládat hodnoty, jak uav vidí dané místo. matice je cíl diskretizovaný stejnì jako pøi a star hledání.
		//prázdný cíl má hodnotu 100. pokud cíl vidí UAV, vydìlí se hodnota dvìma. Nejmenší souèet je nejlepší.
		//matice UAV má všude 1, jen tam, kam vidí UAV, je 0.5. Pak se prvek po prvku vynásobí s maticí cílù. tím se vydìlí dvìma to, co vidí UAV a zbytek je nedotèen.

		//inicializace matice cílù. Všechny cíle jsou v jedné matici
		int rowCount = floor(worldHeight / elementSize);
		int columnCount = floor(worldWidth / elementSize);

		if (!goalMatrixInitialized)
		{
			goalMatrix = ublas::matrix<double>(rowCount, columnCount, 0);//initializes matrix with 0
			for (auto goal : map->getGoals())
			{
				//filling goal in matrix with initial value
				auto rect = goal->getRectangle();
				auto width = floor(rect->getWidth() / elementSize);
				auto height = floor(rect->getHeight() / elementSize);
				ublas::subrange(goalMatrix, rect->getX(), rect->getX() + width, rect->getY(), rect->getY() + height) = ublas::matrix<double>(width, height, initialValue);
			}
			goalMatrixInitialized = true;
		}


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
			int minX = max<int>(floor(loc->getX() - halfCameraX), 0);
			int maxX = min<int>(floor(loc->getX() + halfCameraX), rowCount - 1);
			int minY = max<int>(floor(loc->getY() - halfCameraY), 0);
			int maxY = min<int>(floor(loc->getY() + halfCameraY), columnCount - 1);
			ublas::subrange(matrix, minX, maxX, minY, maxY) = ublas::matrix<double>(uavCameraX, uavCameraY, uavInitValue);
		}

		//vytvoøení prùniku nenulových hodnot a úprava hodnot matice mapy
		for (auto uavMatrix : uavMatrixes)
		{
			auto uav = uavMatrix.first;
			auto matrix = uavMatrix.second;

			goalMatrix = element_prod(goalMatrix, matrix);
		}

		return sum(prod(ublas::scalar_vector<double>(goalMatrix.size1()), goalMatrix));	//sum of whole matrix, for weird reason, I must multiply here (prod), fuck you C++ http://stackoverflow.com/questions/24398059/how-do-i-sum-all-elements-in-a-ublas-matrix
	}

}