#include "LoggerInterface.h"
#include "Path.h"

namespace App
{

	LoggerInterface::LoggerInterface()
	{
	}


	LoggerInterface::~LoggerInterface()
	{
	}

	void LoggerInterface::logSelectedMap(Map* map, int worldWidth, int worldHeight)
	{
	}

	void LoggerInterface::logMapGrid(std::vector<std::vector<Grid>> mapGrid)
	{
	}

	void LoggerInterface::logGuidingPaths(std::vector<Path*> paths, Node* start, std::vector<Node*> ends)
	{
	}
}
