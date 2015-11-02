#include "GuidingPathFactory.h"
#include "VCollide/ColDetect.h"

namespace App
{

	GuidingPathFactory::GuidingPathFactory(LoggerInterface* logger) : logger(logger)
	{
	}

	GuidingPathFactory::~GuidingPathFactory()
	{
	}

	std::vector<App::Path*> GuidingPathFactory::createGuidingPaths(std::vector<Node*> nodes, Node start, Node end)
	{
//		logger->logMapGrid(mapGrid);
		//here comes A* algorithm
		auto paths = std::vector<App::Path*>();
		return paths;
	}

}
