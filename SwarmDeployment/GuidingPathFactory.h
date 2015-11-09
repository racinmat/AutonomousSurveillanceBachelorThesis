#pragma once
#include "Path.h"
#include "Map.h"
#include "LoggerInterface.h"
#include <set>
#include "PathFindingAlgorithm.h"

namespace App
{

	class GuidingPathFactory
	{
	public:
		GuidingPathFactory(LoggerInterface* logger);
		virtual ~GuidingPathFactory();
		std::vector<std::vector<std::shared_ptr<App::Node>>> createGuidingPaths(std::vector<std::shared_ptr<App::Node>> nodes, std::shared_ptr<App::Node> start, std::vector<std::shared_ptr<App::Node>> ends);
	
	protected:
		LoggerInterface* logger;
		PathFindingAlgorithm* algorithm;
	};

}
