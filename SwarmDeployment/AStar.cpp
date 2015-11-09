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

	std::shared_ptr<App::Path> AStar::findPath(std::vector<std::shared_ptr<App::Node>> nodes, std::shared_ptr<App::Node> start, std::shared_ptr<App::Node> end)
	{
		//here is A* algorithm
		opened = OpenedSet();
		closed = ClosedSet();
		auto current = std::make_shared<NodeWrapper>(nullptr, start, end);
		auto endWrapper = std::make_shared<NodeWrapper>(nullptr, end, end);
		int counter = 0;
		int limit = 10000;
		std::set<std::shared_ptr<NodeWrapper>> neighbors;
		while ((*current.get()) != (*endWrapper.get())) {
			//expanding and picking best next node
			neighbors = current->expand(end);
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
			//end of expanding and picking best next node

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

		//hydrating path from node wrappers
		auto path = std::make_shared<App::Path>();
		for (auto wrapper : way) {
			path->addToStart(wrapper->getNode());
		}
		return path;
	}


}