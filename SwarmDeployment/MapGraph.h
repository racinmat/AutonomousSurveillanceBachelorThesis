#pragma once
#include <vector>
#include "Node.h"

using namespace std;

namespace App
{

	class MapGraph
	{
	public:
		MapGraph(vector<shared_ptr<Node>> nodes, shared_ptr<Node> startNode, vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> endNodes);
		virtual ~MapGraph();
		vector<shared_ptr<Node>> getAllNodes();
		shared_ptr<Node> getStartNode();
		vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> getEndNodes();

	protected:
		vector<shared_ptr<Node>> nodes;
		shared_ptr<Node> startNode;
		vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> endNodes;
	};

}