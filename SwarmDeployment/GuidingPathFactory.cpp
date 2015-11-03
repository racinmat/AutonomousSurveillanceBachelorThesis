#include "GuidingPathFactory.h"

namespace App
{

	GuidingPathFactory::GuidingPathFactory(LoggerInterface* logger) : logger(logger)
	{
	}

	GuidingPathFactory::~GuidingPathFactory()
	{
	}

	std::vector<App::Path*> GuidingPathFactory::createGuidingPaths(std::vector<Node*> nodes, Node* start, std::vector<Node*> ends) //more ends for more AoI
	{
		auto paths = std::vector<App::Path*>(ends.size());
		for (size_t i = 0; i < ends.size(); i++)
		{
			paths[i] = findPath(nodes, start, ends[i]);
		}
		return paths;
	}

	Path* GuidingPathFactory::findPath(std::vector<Node*> nodes, Node* start, Node* end)
	{
		//here is A* algorithm
		//sauce: http://code.activestate.com/recipes/577457-a-star-shortest-path-algorithm/
		
		std::vector<Node*> open;
		std::vector<Node*> closed;
		Node* current = start;

		while (current != end) {
			current = examineNextNode(current, end);
		}

		return new Path();
	}

	///Returns new current node, best neighbor of all opened nodes.
	Node* GuidingPathFactory::examineNextNode(Node* current, Node* end)
	{
		auto neighbors = current->getNeighbors();
		for (auto neighbor : neighbors)
		{
			if (opened.count(neighbor) == 0)	//set does not have "contains" method. Fuck you, C++.
			{
				if (closed.count(neighbor) == 0)
				{
					opened.insert(neighbor);
				}
			}
		}
		closed.insert(current);
		//todo: dodělat výběr nejlepší node
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
