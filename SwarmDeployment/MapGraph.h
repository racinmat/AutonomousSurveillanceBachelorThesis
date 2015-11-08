#pragma once
#include <vector>
#include "Node.h"

namespace App
{

	class MapGraph
	{
	public:
		MapGraph(std::vector<std::shared_ptr<App::Node>> nodes, std::shared_ptr<App::Node> startNode, std::vector<std::shared_ptr<App::Node>> endNodes);
		virtual ~MapGraph();
		std::vector<std::shared_ptr<App::Node>> getAllNodes();
		std::shared_ptr<App::Node> getStartNode();
		std::vector<std::shared_ptr<App::Node>> getEndNodes();

	protected:
		std::vector<std::shared_ptr<App::Node>> nodes;
		std::shared_ptr<App::Node> startNode;
		std::vector<std::shared_ptr<App::Node>> endNodes;
	};

}