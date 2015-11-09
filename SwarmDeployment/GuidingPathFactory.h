#pragma once
#include "Path.h"
#include "Map.h"
#include "LoggerInterface.h"
#include <set>
#include "PathFindingAlgorithm.h"

using namespace std;

namespace App
{

	class GuidingPathFactory
	{
	public:
		GuidingPathFactory(shared_ptr<LoggerInterface> logger);
		virtual ~GuidingPathFactory();
		std::vector<std::vector<std::shared_ptr<Node>>> createGuidingPaths(std::vector<std::shared_ptr<Node>> nodes, std::shared_ptr<Node> start, std::vector<std::shared_ptr<Node>> ends);
	
	protected:
		shared_ptr<LoggerInterface> logger;
		shared_ptr<PathFindingAlgorithm> algorithm;
	};

}
