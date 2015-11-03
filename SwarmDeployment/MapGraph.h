#pragma once
#include <vector>
#include "Node.h"

namespace App
{

	class MapGraph
	{
	public:
		MapGraph(std::vector<Node*> nodes, Node* startNode, std::vector<Node*> endNodes);
		virtual ~MapGraph();
		std::vector<Node*> getAllNodes();
		Node* getStartNode();
		std::vector<Node*> getEndNodes();

	protected:
		std::vector<Node*> nodes;
		Node* startNode;
		std::vector<Node*> endNodes;
	};

}