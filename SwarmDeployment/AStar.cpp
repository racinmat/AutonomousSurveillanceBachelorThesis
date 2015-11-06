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
		//here is A* algorithm
		NodeWrapper* current = new NodeWrapper(nullptr, start, end);
		NodeWrapper* endWrapper = new NodeWrapper(nullptr, end, end);
		while (current != endWrapper) {
			current = examineNextNode(current, end);
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

	App::Path* AStar::getNodesFromWrappers(std::set<NodeWrapper*> wrappers)
	{
		App::Path* path = new App::Path();
		for (NodeWrapper* wrapper : wrappers) {
			path->addToStart(wrapper->getNode());
		}
		return path;
	}
}