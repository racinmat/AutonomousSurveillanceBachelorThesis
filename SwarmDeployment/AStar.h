#pragma once
#include "PathFindingAlgorithm.h"
#include <set>
#include "NodeSet.h"

namespace AStar
{

	class AStar : public App::PathFindingAlgorithm
	{
	public:
		AStar();
		virtual ~AStar() override;
		App::Path* findPath(std::vector<App::Node*> nodes, App::Node* start, App::Node* end) override;

	protected:
		NodeSet opened;
		NodeSet closed;
		App::Node* examineNextNode(App::Node* current, App::Node* end);
	};

}