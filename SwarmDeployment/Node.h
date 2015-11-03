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

	protected:
		Point* point;
		double cost;
		std::array<Node*, 8> neighbors;	//better than std::vector because here I need only fixed amount of elements. The array is immutable in this case.
		Grid gridType;
	};

}