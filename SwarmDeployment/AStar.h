#pragma once
#include "PathFindingAlgorithm.h"
#include "NodeSet.h"
#include "ClosedSet.h"
#include "OpenedSet.h"
#include "memory"

namespace AStar
{

	class AStar : public App::PathFindingAlgorithm
	{
	public:
		AStar();
		virtual ~AStar() override;
		std::vector<std::shared_ptr<App::Node>> findPath(std::vector<std::shared_ptr<App::Node>> nodes, std::shared_ptr<App::Node> start, std::shared_ptr<App::Node> end) override;

	protected:
		OpenedSet opened;
		ClosedSet closed;
	};

}