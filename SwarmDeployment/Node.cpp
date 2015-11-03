#include "Node.h"

namespace App
{
	Node::Node(Point* point, Grid gridType, double cost) :
		point(point), gridType(gridType), cost(cost)
	{
	}

	Node::Node(Point* point, Grid gridType) :
		point(point), gridType(gridType), cost(1)
	{
	}

	Node::~Node()
	{
	}

	Point* Node::getPoint() const
	{
		return point;
	}

	double Node::getCost() const
	{
		return cost;
	}

	std::array<Node*, 8> Node::getNeighbors() const
	{
		return neighbors;
	}

	void Node::addNeighbor(Node* node, int position)
	{
		if (position >= neighbors.size())
		{
			throw "Invalid position. Maximal position is " + (neighbors.size() - 1);
		}
		neighbors[position] = node;
	}

	void Node::increaseCost(double increase)
	{
		cost += increase;
	}

	Grid Node::getGridType() const
	{
		return gridType;
	}

	bool Node::contains(int x, int y, int distance)
	{
		return x - point->getX() <= distance && y - point->getY() <= distance;
	}

}