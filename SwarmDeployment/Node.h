#pragma once
#include "Point.h"
#include <array>
#include "Map.h"

namespace App
{

	class Node
	{
	public:
		Node(Point* point, Grid gridType, double cost);
		Node(Point* point, Grid gridType);
		virtual ~Node();
		Point* getPoint() const;
		double getCost() const;
		std::array<Node*, 8> getNeighbors() const;
		void addNeighbor(Node* node, int position);
		void increaseCost(double increase);
		Grid getGridType() const;
		bool contains(int x, int y, int distance);
		bool operator<(const Node& another); //needed for comparation when using count method of std::set
		bool operator>(const Node& another); //If nodes are same, wrappers should also behave as same.
		//I suppose count method uses <, > operators, it is written here: http://stackoverflow.com/questions/8118148/how-does-count-in-c-stl-set-work
	protected:
		Point* point;
		double cost;
		std::array<Node*, 8> neighbors;	//better than std::vector because here I need only fixed amount of elements. The array is immutable in this case.
		Grid gridType;
	};

}