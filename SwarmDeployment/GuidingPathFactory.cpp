#include "GuidingPathFactory.h"
#include "AStar.h"

namespace App
{

	GuidingPathFactory::GuidingPathFactory(shared_ptr<LoggerInterface> logger) : logger(logger), algorithm(make_shared<AStar::AStar>())
	{
	}

	GuidingPathFactory::~GuidingPathFactory()
	{
	}

	std::vector<std::shared_ptr<Path>> GuidingPathFactory::createGuidingPaths(std::vector<std::shared_ptr<Node>> nodes, std::shared_ptr<Node> start, std::vector<std::shared_ptr<Node>> ends) //more ends for more AoI
	{
		auto paths = std::vector<std::shared_ptr<Path>>(ends.size());
		for (size_t i = 0; i < ends.size(); i++)
		{
			paths[i] = algorithm->findPath(nodes, start, ends[i]);
		}
//		logger->logGuidingPaths(paths, start, ends);
		return paths;
	}

}
