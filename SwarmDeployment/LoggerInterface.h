#pragma once
#include "Map.h"
#include "Path.h"

namespace App
{

	class LoggerInterface
	{
	public:
		LoggerInterface();
		virtual ~LoggerInterface();
		virtual void logSelectedMap(Map* map, int worldWidth, int worldHeight);
		virtual void logMapGrid(std::vector<std::vector<Grid>> mapGrid);
		virtual void logGuidingPaths(std::vector<Path*> paths, Node* start, std::vector<Node*> ends);
	};

}