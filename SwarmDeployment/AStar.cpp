#include "AStar.h"

using namespace std;

namespace AStar
{

	AStar::AStar(shared_ptr<App::LoggerInterface> logger) : logger(logger)
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
			int x = current->getX();
			int y = current->getY();
			auto neighbors = current->expand(end);
			for (auto neighbor : neighbors)
			{
				if (!opened.contains(neighbor) && !closed.contains(neighbor))
				{
					opened.insert(neighbor);
				}
			}
			closed.insert(current);

			if (opened.size() == 0)	//kontrola je pøed pollBest, protože po pollBest, pokud je v opened pouze 1 prvek, je 0 prvkù a vyhodí se výjimka i když je v current zatím neexpandovaná node vedoucí k cíli
			{
				throw "No opened nodes";
				break;
			}

			do {
				current = opened.pollBest();
			} while (closed.contains(current));
			logger->logAStarNode(current);
			//end of expanding and picking best next node

			counter++;
			if (counter > limit)
			{
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
