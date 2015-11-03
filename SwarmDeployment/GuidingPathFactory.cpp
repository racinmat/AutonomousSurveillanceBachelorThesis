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
			paths[i] = findPath(nodes, start, ends[i]);
		}
		return paths;
	}

	Path* GuidingPathFactory::findPath(std::vector<Node*> nodes, Node* start, Node* end)
	{
		//here is A* algorithm
		//sauce: http://code.activestate.com/recipes/577457-a-star-shortest-path-algorithm/
		

		return new Path();
	}
}
