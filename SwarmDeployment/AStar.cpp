#include "AStar.h"


namespace AStar
{


	AStar::AStar()
	{
	}


	AStar::~AStar()
	{
	}

	App::Path* AStar::findPath(std::vector<App::Node*> nodes, App::Node* start, App::Node* end)
	{
		opened = OpenedSet();
		closed = ClosedSet();
		//here is A* algorithm
		NodeWrapper* current = new NodeWrapper(nullptr, start, end);
		NodeWrapper* endWrapper = new NodeWrapper(nullptr, end, end);
		int counter = 0;
		int limit = 4000;
		while ((*current) != (*endWrapper)) {
			current = examineNextNode(current, end);
			counter++;
			if (counter > limit)
			{
				break;
			}
			if (opened.size() == 0)
			{
				throw "No opened nodes";
				break;
			}
		}

		auto way = current->getWay();
		return getNodesFromWrappers(way);
	}

	///Returns new current node, best neighbor of all opened nodes.
	NodeWrapper* AStar::examineNextNode(NodeWrapper* current, App::Node* end)
	{
		auto neighbors = current->expand(end);
		for (auto neighbor : neighbors)
		{
			if (!opened.contains(neighbor))	//set does not have "contains" method. Fuck you, C++.
			{
				if (!closed.contains(neighbor))
				{
					opened.insert(neighbor);
				}
			}
		}
		closed.insert(current);
		do {
			current = opened.pollBest();
		} while (closed.contains(current));
		return current;
	}

	App::Path* AStar::getNodesFromWrappers(std::vector<NodeWrapper*> wrappers)
	{
		App::Path* path = new App::Path();
		for (NodeWrapper* wrapper : wrappers) {
			path->addToStart(wrapper->getNode());
		}
		return path;
	}
}