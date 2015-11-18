#pragma once
#include "Point.h"
#include <array>
#include "Map.h"
#include <memory>
#include "Enums.h"

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

		//todo: oddělit node pro path od node pro rrt. pro rrt to pojmenovat state
		//todo: přesunout do konfigurace , až to budu refactorovat. Udělat z toho protected proměnné
		static const int input_samples_phi = 3; // Number of steering inputs. Počet úhlů, o které může UAV zatočit mezi iteracemi, nyní je to: rovně, dole, doprava
		static const int input_samples_dist = 1;	//Number of forward speed inputs. Počet rychlostí, které může UAV mít mezi iteracemi
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