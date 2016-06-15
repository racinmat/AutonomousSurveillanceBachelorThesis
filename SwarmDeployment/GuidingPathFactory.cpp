#include "GuidingPathFactory.h"
#include "AStar.h"

namespace App
{

	GuidingPathFactory::GuidingPathFactory(shared_ptr<LoggerInterface> logger) : logger(logger), algorithm(make_shared<AStar::AStar>(logger))
	{
	}

	GuidingPathFactory::~GuidingPathFactory()
	{
	}

	vector<shared_ptr<Path>> GuidingPathFactory::createGuidingPaths(vector<shared_ptr<Node>> nodes, shared_ptr<Node> start, vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> ends) //more ends for more AoI
	{
		auto paths = vector<shared_ptr<Path>>(ends.size());
		for (size_t i = 0; i < ends.size(); i++)
		{
			paths[i] = algorithm->findPath(nodes, start, get<0>(ends[i]), get<1>(ends[i]));
		}
		logger->logGuidingPaths(paths, start, ends);
		return paths;
	}

	void GuidingPathFactory::setLogger(const shared_ptr<LoggerInterface> logger_interface)
	{
		logger = logger_interface;
	}
}
