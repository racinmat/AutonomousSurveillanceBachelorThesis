#pragma once
#include "Point.h"
#include <array>

namespace App
{

	class Node
	{
	public:
		Node(Point* point, double cost);
		Node(Point* point);
		virtual ~Node();
		Point* getPoint() const;
		double getCost() const;
		std::array<Node*, 8> getNeighbors() const;
		void addNeighbor(Node* node, int position);

	protected:
		Point* point;
		double cost;
		std::array<Node*, 8> neighbors;	//better than std::vector because here I need only fixed amount of elements. The array is immutable in this case.
	};

}