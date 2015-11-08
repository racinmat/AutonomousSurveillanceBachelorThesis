#include "MapGraph.h"

namespace App
{

	App::MapGraph::MapGraph(std::vector<std::shared_ptr<App::Node>> nodes, std::shared_ptr<App::Node> startNode, std::vector<std::shared_ptr<App::Node>> endNodes) :
		nodes(nodes), startNode(startNode), endNodes(endNodes)
	{
	}

	MapGraph::~MapGraph()
	{
	}

	std::vector<std::shared_ptr<App::Node>> MapGraph::getAllNodes()
	{
		return nodes;
	}

	std::shared_ptr<App::Node> MapGraph::getStartNode()
	{
		return startNode;
	}

	std::vector<std::shared_ptr<App::Node>> MapGraph::getEndNodes()
	{
		return endNodes;
	}

}