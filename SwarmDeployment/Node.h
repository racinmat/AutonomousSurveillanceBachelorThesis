#pragma once
#include "Point.h"
#include <array>
#include "Map.h"
#include <memory>
#include <cmath>

using namespace std;

namespace App
{

	class Node
	{
	public:
		Node(shared_ptr<Point> point, Grid gridType, double cost);
		Node(shared_ptr<Point> point, Grid gridType);
		virtual ~Node();
		shared_ptr<Point> getPoint() const;
		double getCost() const;
		std::vector<std::shared_ptr<Node>> getNeighbors() const;
		void addNeighbor(std::shared_ptr<Node> node, int position);
		void increaseCost(double increase);
		Grid getGridType() const;
		bool contains(int x, int y, int distance);
		int index;	//todo: zjistit, k čemu to má být, používá se to v rrt_path
		vector<bool> used_inputs; // na začátku pole false, o délce number_of_inputs
		
		//todo: přesunout do konfigurace , až to budu refactorovat. Udělat z toho protected proměnné
		static const int input_samples_phi = 3; // Number of steering inputs
		static const int input_samples_dist = 1;	//Number of forward speed inputs
		static const int number_of_uavs = 4;	//todo: přebrat z konfigurace v konstruktoru
		//static const int number_of_inputs = pow((input_samples_phi * input_samples_dist), number_of_uavs);//todo: přesunout pryč, zprovoznit. je zatím je pro délku vektoru used_inputs
		vector<shared_ptr<PointParticle>> uavs; //spojení proměnných loc a rot z Node objektu z matlabu. nejspíš node bude jiná pro rrt path a pro diskretizaci na nalezen guiding path

	protected:
		shared_ptr<Point> point;
		double cost;
		Grid gridType;
		std::vector<std::shared_ptr<Node>> neighbors;	//vector, because each node has different amount of neighbors and array with size 8 returns some null values, when node has less neighbors than 8
	};

}