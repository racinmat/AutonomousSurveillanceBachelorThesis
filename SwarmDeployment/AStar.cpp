#include "AStar.h"

using namespace std;

namespace AStar
{


	AStar::AStar()
	{
	}


	AStar::~AStar()
	{
	}

	shared_ptr<App::Path> AStar::findPath(vector<shared_ptr<App::Node>> nodes, shared_ptr<App::Node> start, shared_ptr<App::Node> end)
	{
		//here is A* algorithm
		opened = OpenedSet();
		closed = ClosedSet();

		shared_ptr<NodeWrapper> current;
		current = make_shared<NodeWrapper>(nullptr, start, end);
		int counter = 0;
		int limit = 10000;
		while ((*current.get()) != (*end.get())) {
			//expanding and picking best next node
			auto neighbors = current->expand(end);
			for (auto neighbor : neighbors)
			{
				if (!opened.contains(neighbor) && !closed.contains(neighbor))
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

//		hydrating path from node wrappers
		auto path = make_shared<App::Path>();
		for (size_t i = 0; i < way.size(); i++)
		{
			path->addToEnd(way[i]->getNode());
		}

		return path;
	}

}
