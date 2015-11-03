#include "MapGraph.h"

namespace App
{

	App::MapGraph::MapGraph(std::vector<Node*> nodes, Node* startNode, std::vector<Node*> endNodes) : 
		nodes(nodes), startNode(startNode), endNodes(endNodes)
	{
	}

	MapGraph::~MapGraph()
	{
	}

	std::vector<App::Node*> MapGraph::getAllNodes()
	{
		return nodes;
	}

	App::Node* MapGraph::getStartNode()
	{
		return startNode;
	}

	std::vector<App::Node*> MapGraph::getEndNodes()
	{
		return endNodes;
	}

}