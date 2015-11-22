#include "Node.h"
#include <memory>

namespace App
{
	Node::Node(shared_ptr<Point> point, Grid gridType, double cost) :
		point(point), cost(cost), gridType(gridType)
	{
	}

	Node::Node(shared_ptr<Point> point, Grid gridType) :
		point(point), cost(1), gridType(gridType)
	{
	}

	Node::~Node()
	{
	}

	shared_ptr<Point> Node::getPoint() const
	{
		return point;
	}

	double Node::getCost() const
	{
		return cost;
	}

	std::vector<std::shared_ptr<Node>> Node::getNeighbors() const
	{
		return neighbors;
	}

	void Node::addNeighbor(std::shared_ptr<Node> node, int position)
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
		return abs(x - point->getX()) <= distance && abs(y - point->getY()) <= distance;
	}

	bool operator==(const Node& lhs, const Node& rhs)
	{
		return (*lhs.point.get()) == (*rhs.point.get())
			&& lhs.cost == rhs.cost
			&& lhs.gridType == rhs.gridType;
	}

	bool operator!=(const Node& lhs, const Node& rhs)
	{
		return !(lhs == rhs);
	}

	std::size_t hash_value(const Node& obj)
	{
		std::size_t seed = 0x79A3FD8E;
		seed ^= (seed << 6) + (seed >> 2) + 0x0FB09D6C + hash_value(*obj.point.get());
		seed ^= (seed << 6) + (seed >> 2) + 0x1D579C4C + static_cast<std::size_t>(obj.cost);
		seed ^= (seed << 6) + (seed >> 2) + 0x0E0AFB73 + static_cast<std::size_t>(obj.gridType);
		return seed;
	}
}