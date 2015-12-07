#include "Node.h"
#include <memory>
#include <algorithm>

namespace App
{
	Node::Node(shared_ptr<Point> point, Grid gridType, double cost) :
		point(point), cost(cost), gridType(gridType), distanceToObstacle(INT32_MAX)
	{
	}

	Node::Node(shared_ptr<Point> point, Grid gridType) :
		point(point), cost(1), gridType(gridType), distanceToObstacle(INT32_MAX)
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

	vector<shared_ptr<Node>> Node::getNeighbors() const
	{
		auto neighborNodes = vector<shared_ptr<Node>>(neighbors.size());
		transform(neighbors.begin(), neighbors.end(), neighborNodes.begin(), [](tuple<shared_ptr<Node>, bool> i) {return get<0>(i); } );	//mapování pomocí lambda funkce, vytáhne z tuplu jen nody
		return neighborNodes;
	}

	vector<shared_ptr<Node>> Node::getNeighborsWithoutObstacles() const
	{
		auto neighborsWithoutObstacles = vector<shared_ptr<Node>>();	//copy_if nechává v poli prázdné prvky a já nevím, jak se jich zbavit
		for (auto tuple : neighbors)
		{
			auto node = get<0>(tuple);
			if (node->getGridType() != Grid::Obstacle)
			{
				neighborsWithoutObstacles.push_back(node);
			}
		}
		return neighborsWithoutObstacles;
	}

	vector<shared_ptr<Node>> Node::getDirectNeighbors() const
	{
		auto directNeighbors = vector<shared_ptr<Node>>();	//copy_if nechává v poli prázdné prvky a já nevím, jak se jich zbavit
		for (auto tuple : neighbors)
		{
			bool isDiagonal = get<1>(tuple);
			auto node = get<0>(tuple);
			if (!isDiagonal)	
			{
				directNeighbors.push_back(node);
			}
		}
		return directNeighbors;
	}

	void Node::addNeighbor(shared_ptr<Node> node, bool isDiagonal)
	{
		auto tuple = make_tuple(node, isDiagonal);
		neighbors.push_back(tuple);
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
		return abs(x - point->getX()) <= distance && abs(y - point->getY()) <= distance;	//abs je pouze pro integery
	}

	bool Node::contains(Point point, int distance)
	{
		return contains(point.getX(), point.getY(), distance);
	}

	bool Node::contains(shared_ptr<Point> point, int distance)
	{
		return contains(*point.get(), distance);
	}

	double Node::getDistanceToObstacle() const
	{
		return distanceToObstacle;
	}

	void Node::setDistanceToObstacle(const double distance_to_obstacle)
	{
		distanceToObstacle = distance_to_obstacle;
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

	size_t hash_value(const Node& obj)
	{
		size_t seed = 0x79A3FD8E;
		seed ^= (seed << 6) + (seed >> 2) + 0x0FB09D6C + hash_value(*obj.point.get());
		seed ^= (seed << 6) + (seed >> 2) + 0x1D579C4C + static_cast<size_t>(obj.cost);
		seed ^= (seed << 6) + (seed >> 2) + 0x0E0AFB73 + static_cast<size_t>(obj.gridType);
		return seed;
	}
}