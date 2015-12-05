#pragma once
#include "PathFindingAlgorithm.h"
#include "ClosedSet.h"
#include "OpenedSet.h"
#include "memory"
#include "LoggerInterface.h"
#include "Path.h"

namespace AStar
{

	class AStar : public PathFindingAlgorithm
	{
	public:
		explicit AStar(shared_ptr<LoggerInterface> logger);
		virtual ~AStar() override;
		virtual shared_ptr<Path> findPath(vector<shared_ptr<Node>> nodes, shared_ptr<Node> start, shared_ptr<Node> end, shared_ptr<GoalInterface> goal) override;

	protected:
		OpenedSet opened;
		ClosedSet closed;
		shared_ptr<LoggerInterface> logger;
	};

}