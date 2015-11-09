#include "AStar.h"
#include "memory"

using namespace std;

namespace AStar
{


	AStar::AStar()
	{
	}


	AStar::~AStar()
	{
	}

	vector<shared_ptr<App::Node>> AStar::findPath(vector<shared_ptr<App::Node>> nodes, shared_ptr<App::Node> start, shared_ptr<App::Node> end)
	{
		//here is A* algorithm
		opened = OpenedSet();
		closed = ClosedSet();
		vector<shared_ptr<NodeWrapper>> way;
		vector<shared_ptr<App::Node>> path;

		shared_ptr<NodeWrapper> current;
		current = make_shared<NodeWrapper>(nullptr, start, end);
		int counter = 0;
		int limit = 10000;
		while ((*current.get()) != (*end.get())) {
			//expanding and picking best next node
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

		way = current->getWay();

//		hydrating path from node wrappers
		path = vector<shared_ptr<App::Node>>(way.size());
		for (size_t i = 0; i < way.size(); i++)
		{
			path[i] = way[i]->getNode();
		}
//		auto path = vector<shared_ptr<App::Node>>(2);
//		path[0] = start;
//		path[1] = end;

		return path;
	}

}
