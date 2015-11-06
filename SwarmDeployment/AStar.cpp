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
		//sauce: http://code.activestate.com/recipes/577457-a-star-shortest-path-algorithm/

		std::vector<App::Node*> open;
		std::vector<App::Node*> closed;
		NodeWrapper* current = new NodeWrapper(nullptr, start, end);
		NodeWrapper* endWrapper = new NodeWrapper(nullptr, end, end);
		while (current != endWrapper) {
			current = examineNextNode(current, end);
		}

		return new App::Path();//todo: dodìlat
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
		//todo: dodìlat výbìr nejlepší node
	}

	//private void examineNextNode(Node endNode, double distanceCoef) {
	//		List<NodeWrapper> expanded = current.expand(endNode, distanceCoef, nodeExpansions);
	//		for (NodeWrapper expandedNode : expanded) {
	//			if (!opened.contains(expandedNode)) {
	//				if (!closed.contains(expandedNode)) {
	//					opened.add(expandedNode);
	//				}
	//			}
	//		}
	//		closed.add(current);
	//		do {
	//			current = opened.pollBest();
	//		} while (closed.contains(current));
	//	}


}