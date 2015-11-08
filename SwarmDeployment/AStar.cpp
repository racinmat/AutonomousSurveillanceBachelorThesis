#include "AStar.h"
#include "../../../../../../../Program Files (x86)/Microsoft Visual Studio 14.0/VC/include/memory"


namespace AStar
{


	AStar::AStar()
	{
	}


	AStar::~AStar()
	{
	}

	App::Path* AStar::findPath(std::vector<std::shared_ptr<App::Node>> nodes, std::shared_ptr<App::Node> start, std::shared_ptr<App::Node> end)
	{
		//here is A* algorithm
		opened = OpenedSet();
		closed = ClosedSet();
		auto current = std::make_shared<NodeWrapper>(nullptr, start, end);
		auto endWrapper = std::make_shared<NodeWrapper>(nullptr, end, end);
		int counter = 0;
		int limit = 10000;
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

		auto path = getNodesFromWrappers(way);

		return path;
	}

	///Returns new current node, best neighbor of all opened nodes.
	std::shared_ptr<NodeWrapper> AStar::examineNextNode(std::shared_ptr<NodeWrapper> current, std::shared_ptr<App::Node> end)
	{
		auto neighbors = current->expand(end);
		for (auto neighbor : neighbors)
		{
			if (!opened.contains(neighbor) && !closed.contains(neighbor))	//set does not have "contains" method. Fuck you, C++.
			{
				opened.insert(neighbor);
			}
		}
		closed.insert(current);
		do {
			current = opened.pollBest();
		} while (closed.contains(current));
		return current;
	}

	App::Path* AStar::getNodesFromWrappers(std::vector<std::shared_ptr<NodeWrapper>> wrappers)
	{
		App::Path* path = new App::Path();
		for (auto wrapper : wrappers) {
			path->addToStart(wrapper->getNode());
		}
		return path;
	}
}