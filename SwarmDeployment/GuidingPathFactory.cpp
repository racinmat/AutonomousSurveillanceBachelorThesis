#include "GuidingPathFactory.h"

namespace App
{

	GuidingPathFactory::GuidingPathFactory(LoggerInterface* logger) : logger(logger)
	{
	}

	GuidingPathFactory::~GuidingPathFactory()
	{
	}

	std::vector<App::Path*> GuidingPathFactory::createGuidingPaths(std::vector<Node*> nodes, Node* start, std::vector<Node*> ends) //more ends for more AoI
	{
		auto paths = std::vector<App::Path*>(ends.size());
		for (size_t i = 0; i < ends.size(); i++)
		{
			paths[i] = algorithm->findPath(nodes, start, ends[i]);
		}
		return paths;
	}

}
