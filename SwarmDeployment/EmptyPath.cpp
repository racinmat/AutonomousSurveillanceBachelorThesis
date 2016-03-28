#include "EmptyPath.h"

namespace App
{

	EmptyPath::EmptyPath(shared_ptr<GoalInterface> goal) : Path(goal)
	{
		nodes = std::vector<std::shared_ptr<Node>>();
	}

	EmptyPath::~EmptyPath()
	{
	}

	std::vector<std::shared_ptr<Node>> EmptyPath::getNodes() const
	{
		return nodes;
	}

	int EmptyPath::getSize() const
	{
		return 0;
	}

	shared_ptr<Node> EmptyPath::get(int i)
	{
		throw "This is empty path";
	}

	bool EmptyPath::hasNext(shared_ptr<Node> node)
	{
		throw "This is empty path";
	}

	shared_ptr<Node> EmptyPath::getNext(shared_ptr<Node> node)
	{
		throw "This is empty path";
	}

	bool EmptyPath::isFirstCloserOrSameToEnd(shared_ptr<Node> first, shared_ptr<Node> second)
	{
		throw "This is empty path";
	}

	int EmptyPath::getIndex(shared_ptr<Node> node)
	{
		throw "This is empty path";
	}

	shared_ptr<GoalInterface> EmptyPath::getGoal() const
	{
		return goal;
	}

	void EmptyPath::addToEnd(std::shared_ptr<Node> node)
	{
		throw "This is empty path";
	}

}