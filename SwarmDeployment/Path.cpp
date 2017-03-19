#include "Path.h"

namespace App
{

	Path::Path(shared_ptr<GoalInterface> goal) : goal(goal)
	{
		nodes = std::vector<std::shared_ptr<Node>>();
	}

	Path::~Path()
	{
	}

	std::vector<std::shared_ptr<Node>> Path::getNodes() const
	{
		return nodes;
	}

	int Path::getSize() const
	{
		return nodes.size();
	}

	shared_ptr<Node> Path::get(int i)
	{
		return nodes[i];
	}

	bool Path::hasNext(shared_ptr<Node> node)
	{
		return nodes[nodes.size() - 1] != node;
	}

	shared_ptr<Node> Path::getNext(shared_ptr<Node> node)
	{
		for (size_t i = 0; i < nodes.size() - 1; i++)
		{
			auto iter = nodes[i];
			if (iter == node)
			{
				return nodes[i + 1];
			}
		}
		throw "Node not found";
	}

	bool Path::isFirstCloserOrSameToEnd(shared_ptr<Node> first, shared_ptr<Node> second)
	{
		return getIndex(first) >= getIndex(second);
	}

	size_t Path::hash() const
	{
		size_t seed = 0x083561E3;
		size_t nodesHash = 0x083741A3;
		for (size_t i = 0; i < nodes.size(); i++)
		{
			seed ^= (seed << 6) + (seed >> 2) + 0x074A6BF2 + hash_value(*nodes[i].get());
		}
		seed ^= (seed << 6) + (seed >> 2) + 0x074A6BE1 + nodesHash;
		return seed;
	}

	int Path::getIndex(shared_ptr<Node> node)
	{
		for (size_t i = 0; i < nodes.size(); i++)
		{
			auto iter = nodes[i];
			if (iter == node)
			{
				return i;
			}
		}
		throw "Node not found";
	}

	shared_ptr<GoalInterface> Path::getGoal() const
	{
		return goal;
	}

	void Path::addToEnd(std::shared_ptr<Node> node)
	{
		nodes.push_back(node);
	}

	bool operator==(const Path& lhs, const Path& rhs)
	{
		return lhs.nodes == rhs.nodes;
	}

	bool operator!=(const Path& lhs, const Path& rhs)
	{
		return !(lhs == rhs);
	}

}