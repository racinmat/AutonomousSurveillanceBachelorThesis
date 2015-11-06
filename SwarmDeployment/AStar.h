#pragma once
#include "PathFindingAlgorithm.h"
#include "NodeSet.h"
#include "ClosedSet.h"
#include "OpenedSet.h"

namespace AStar
{

	class AStar : public App::PathFindingAlgorithm
	{
	public:
		AStar();
		virtual ~AStar() override;
		App::Path* findPath(std::vector<App::Node*> nodes, App::Node* start, App::Node* end) override;

	protected:
		OpenedSet opened;
		ClosedSet closed;
		NodeWrapper* examineNextNode(NodeWrapper* current, App::Node* end);
		App::Path* getNodesFromWrappers(std::set<NodeWrapper*> wrappers);
	};

}