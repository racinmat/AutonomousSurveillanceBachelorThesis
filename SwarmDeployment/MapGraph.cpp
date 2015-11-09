#include "MapGraph.h"

namespace App
{

	MapGraph::MapGraph(std::vector<std::shared_ptr<Node>> nodes, std::shared_ptr<Node> startNode, std::vector<std::shared_ptr<Node>> endNodes) :
		nodes(nodes), startNode(startNode), endNodes(endNodes)
	{
	}

	MapGraph::~MapGraph()
	{
	}

	std::vector<std::shared_ptr<Node>> MapGraph::getAllNodes()
	{
		return nodes;
	}

	std::shared_ptr<Node> MapGraph::getStartNode()
	{
		return startNode;
	}

	std::vector<std::shared_ptr<Node>> MapGraph::getEndNodes()
	{
		return endNodes;
	}

}