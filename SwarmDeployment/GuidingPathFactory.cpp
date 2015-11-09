#include "GuidingPathFactory.h"
#include "AStar.h"

namespace App
{

	GuidingPathFactory::GuidingPathFactory(shared_ptr<LoggerInterface> logger) : logger(logger), algorithm(new AStar::AStar())
	{
	}

	GuidingPathFactory::~GuidingPathFactory()
	{
	}

	std::vector<std::vector<std::shared_ptr<Node>>> GuidingPathFactory::createGuidingPaths(std::vector<std::shared_ptr<Node>> nodes, std::shared_ptr<Node> start, std::vector<std::shared_ptr<Node>> ends) //more ends for more AoI
	{
		auto paths = std::vector<std::vector<std::shared_ptr<Node>>>(ends.size());
		for (size_t i = 0; i < ends.size(); i++)
		{
			paths[i] = algorithm->findPath(nodes, start, ends[i]);
		}
//		logger->logGuidingPaths(paths, start, ends);
		return paths;
	}

}
