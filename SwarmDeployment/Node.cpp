#include "Node.h"
#include <memory>

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
		delete point;
	}

	Point* Node::getPoint() const
	{
		return point;
	}

	double Node::getCost() const
	{
		return cost;
	}

	std::vector<std::shared_ptr<App::Node>> Node::getNeighbors() const
	{
		return neighbors;
	}

	void Node::addNeighbor(std::shared_ptr<App::Node> node, int position)
	{
		if (position >= neighbors.size())
		{
			neighbors.push_back(node);
		} else
		{
			neighbors[position] = node;
		}
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
		return std::abs(x - point->getX()) <= distance && std::abs(y - point->getY()) <= distance;
	}

	bool Node::operator<(const Node& another)
	{
		double someBigNumber = 10000;//10000 is arbitrary, I need somehow to compare coordinates by < and > operators. I assume, that map is smaller than 10000 x 10000. 
		//If map gets bigger, feel free to change this number.
		return getPoint()->getX() * someBigNumber + getPoint()->getY() < another.getPoint()->getX() * someBigNumber + another.getPoint()->getY(); // keep the same order
	}

	bool Node::operator>(const Node& another)
	{
		double someBigNumber = 10000;
		return getPoint()->getX() * someBigNumber + getPoint()->getY() > another.getPoint()->getX() * someBigNumber + another.getPoint()->getY(); // keep the same order
	}

}