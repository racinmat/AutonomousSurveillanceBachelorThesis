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
		virtual std::shared_ptr<Path> findPath(std::vector<std::shared_ptr<Node>> nodes, std::shared_ptr<Node> start, std::shared_ptr<Node> end) = 0;
	};

	inline PathFindingAlgorithm::PathFindingAlgorithm()
	{
	}

	inline PathFindingAlgorithm::~PathFindingAlgorithm()
	{
	}
}