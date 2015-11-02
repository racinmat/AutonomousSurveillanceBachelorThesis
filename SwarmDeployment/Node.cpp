#include "Node.h"

namespace App
{
	Node::Node(Point* point, double cost): 
		point(point), cost(cost)
						
	{
	}

	Node::Node(Point* point) : 
		point(point), cost(1)
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
}