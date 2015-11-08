#include "GuidingPathFactory.h"
#include "AStar.h"

namespace App
{

	GuidingPathFactory::GuidingPathFactory(LoggerInterface* logger) : logger(logger), algorithm(new AStar::AStar())
	{
	}

	GuidingPathFactory::~GuidingPathFactory()
	{
	}

	std::vector<App::Path*> GuidingPathFactory::createGuidingPaths(std::vector<std::shared_ptr<App::Node>> nodes, std::shared_ptr<App::Node> start, std::vector<std::shared_ptr<App::Node>> ends) //more ends for more AoI
	{
		auto paths = std::vector<App::Path*>(ends.size());
		for (size_t i = 0; i < ends.size(); i++)
		{
			paths[i] = algorithm->findPath(nodes, start, ends[i]);
		}
		logger->logGuidingPaths(paths, start, ends);
		return paths;
	}

}
