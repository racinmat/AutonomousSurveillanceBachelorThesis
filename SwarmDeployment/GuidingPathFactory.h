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
		vector<shared_ptr<Path>> createGuidingPaths(vector<shared_ptr<Node>> nodes, shared_ptr<Node> start, vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> ends);
	
	protected:
		shared_ptr<LoggerInterface> logger;
		shared_ptr<PathFindingAlgorithm> algorithm;
	};

}
