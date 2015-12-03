#include "MapGraph.h"

namespace App
{

	MapGraph::MapGraph(vector<shared_ptr<Node>> nodes, shared_ptr<Node> startNode, vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> endNodes) :
		nodes(nodes), startNode(startNode), endNodes(endNodes)
	{
	}

	MapGraph::~MapGraph()
	{
	}

	vector<shared_ptr<Node>> MapGraph::getAllNodes()
	{
		return nodes;
	}

	shared_ptr<Node> MapGraph::getStartNode()
	{
		return startNode;
	}

	vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> MapGraph::getEndNodes()
	{
		return endNodes;
	}

}