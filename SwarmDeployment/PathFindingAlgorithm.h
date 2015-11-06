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
		virtual App::Path* findPath(std::vector<Node*> nodes, Node* start, Node* end) = 0;
	};

	inline PathFindingAlgorithm::PathFindingAlgorithm()
	{
	}

	inline PathFindingAlgorithm::~PathFindingAlgorithm()
	{
	}
}