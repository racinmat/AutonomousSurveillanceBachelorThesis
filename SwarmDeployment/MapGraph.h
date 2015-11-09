#pragma once
#include <vector>
#include "Node.h"

using namespace std;

namespace App
{

	class MapGraph
	{
	public:
		MapGraph(vector<shared_ptr<Node>> nodes, shared_ptr<Node> startNode, vector<shared_ptr<Node>> endNodes);
		virtual ~MapGraph();
		vector<shared_ptr<Node>> getAllNodes();
		shared_ptr<Node> getStartNode();
		vector<shared_ptr<Node>> getEndNodes();

	protected:
		vector<shared_ptr<Node>> nodes;
		shared_ptr<Node> startNode;
		vector<shared_ptr<Node>> endNodes;
	};

}