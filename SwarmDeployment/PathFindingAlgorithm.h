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
		virtual std::vector<std::shared_ptr<App::Node>> findPath(std::vector<std::shared_ptr<App::Node>> nodes, std::shared_ptr<App::Node> start, std::shared_ptr<App::Node> end) = 0;
	};

	inline PathFindingAlgorithm::PathFindingAlgorithm()
	{
	}

	inline PathFindingAlgorithm::~PathFindingAlgorithm()
	{
	}
}