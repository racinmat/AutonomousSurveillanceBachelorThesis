#pragma once
#include "Map.h"
#include "Path.h"
#include "memory"

namespace App
{

	class LoggerInterface
	{
	public:
		LoggerInterface();
		virtual ~LoggerInterface();
		virtual void logSelectedMap(std::shared_ptr<Map> map, int worldWidth, int worldHeight);
		virtual void logMapGrid(std::vector<std::vector<Grid>> mapGrid);
		virtual void logGuidingPaths(std::vector<std::shared_ptr<Path>> paths, std::shared_ptr<App::Node> start, std::vector<std::shared_ptr<App::Node>> ends);
		virtual void logText(std::string string);
	};

}