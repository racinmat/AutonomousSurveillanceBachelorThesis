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

	shared_ptr<State> AoICoverageResolver::get_best_fitness(vector<shared_ptr<State>> final_nodes, shared_ptr<Map> map, int elementSize, int worldWidth, int worldHeight)
	{
		goalMatrixInitialized = false;	//invalidace cache p�ed za��tkem hled�n�

		auto finalStatesFitness = unordered_map<shared_ptr<State>, double>();
		for (auto finalState : final_nodes)
		{
			finalStatesFitness[finalState] = fitness_function(finalState, map, elementSize, worldWidth, worldHeight);
		}

		//finding solution with best fitness
		pair<shared_ptr<State>, double> min = *min_element(finalStatesFitness.begin(), finalStatesFitness.end(),
			[](pair<shared_ptr<State>, double> a, pair<shared_ptr<State>, double> b) {return a.second < b.second; }
		);
		return min.first;
	}

	double AoICoverageResolver::fitness_function(shared_ptr<State> final_node, shared_ptr<Map> map, int elementSize, int worldWidth, int worldHeight)
	{
		double initialValue = 100;
		double uavCameraX = (150 / elementSize);
		double uavCameraY = floor(100 / elementSize);
		double halfCameraX = floor(uavCameraX / 2);
		double halfCameraY = floor(uavCameraY / 2);
		double uavInitValue = 1 / 2;

		//do matice (vektoru vektor�) si budu ukl�dat hodnoty, jak uav vid� dan� m�sto. matice je c�l diskretizovan� stejn� jako p�i a star hled�n�.
		//pr�zdn� c�l m� hodnotu 100. pokud c�l vid� UAV, vyd�l� se hodnota dv�ma. Nejmen�� sou�et je nejlep��.
		//matice UAV m� v�ude 1, jen tam, kam vid� UAV, je 0.5. Pak se prvek po prvku vyn�sob� s matic� c�l�. t�m se vyd�l� dv�ma to, co vid� UAV a zbytek je nedot�en.

		//inicializace matice c�l�. V�echny c�le jsou v jedn� matici
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
		auto uavMatrixes = unordered_map<Uav, ublas::matrix<double>, UavHasher>();
		for (auto uav : final_node->uavs)
		{
			uavMatrixes[*uav.get()] = ublas::matrix<double>(rowCount, columnCount, 1);//initializes matrix with 1
		}
		for (auto uavMatrix : uavMatrixes)
		{
			auto uav = uavMatrix.first;
			auto matrix = uavMatrix.second;

			//filling goal in matrix with initial value
			auto loc = uav.getPointParticle()->getLocation();
			ublas::subrange(matrix, loc->getX() - halfCameraX, loc->getX() + halfCameraX, loc->getY() - halfCameraY, loc->getY() + halfCameraY) = ublas::matrix<double>(uavCameraX, uavCameraY, uavInitValue);
		}

		//vytvo�en� pr�niku nenulov�ch hodnot a �prava hodnot matice mapy
		for (auto uavMatrix : uavMatrixes)
		{
			auto uav = uavMatrix.first;
			auto matrix = uavMatrix.second;

			goalMatrix = element_prod(goalMatrix, matrix);
		}

		return sum(prod(ublas::scalar_vector<double>(goalMatrix.size1()), goalMatrix));	//sum of whole matrix, for weird reason, I must multiply here (prod), fuck you C++ http://stackoverflow.com/questions/24398059/how-do-i-sum-all-elements-in-a-ublas-matrix
	}

}