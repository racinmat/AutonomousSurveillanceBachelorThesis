#pragma once
#include <vector>
#include  "Node.h"
#include "Path.h"

namespace App
{

	class PathFindingAlgorithm
	{
	public:
		PathFindingAlgorithm();
		virtual ~PathFindingAlgorithm();
		virtual shared_ptr<Path> findPath(vector<shared_ptr<Node>> nodes, shared_ptr<Node> start, shared_ptr<Node> end, shared_ptr<GoalInterface> goal) = 0;
	};

	inline PathFindingAlgorithm::PathFindingAlgorithm()
	{
	}

	inline PathFindingAlgorithm::~PathFindingAlgorithm()
	{
	}
}