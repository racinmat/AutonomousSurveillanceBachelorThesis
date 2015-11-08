#pragma once
#include "Path.h"
#include "Map.h"
#include "LoggerInterface.h"
#include "MapGraph.h"

namespace App
{
	class MapProcessor
	{
	public:
		MapProcessor(LoggerInterface* logger);
		virtual ~MapProcessor();
		MapGraph* mapToNodes(std::shared_ptr<Map> map, int cellSize, int worldWidth, int worldHeigh, double uavSize);

	protected:
		std::vector<std::vector<Grid>> getMapGrid(std::shared_ptr<Map> map, int cellSize, int worldWidth, int worldHeigh, double uavSize);	//returns 2D matrix as grid of map
		Grid analyzeCell(std::shared_ptr<Map> map, Point leftBottom, Point rightUpper, double uavSize);
		std::vector<std::shared_ptr<App::Node>> gridToNodes(std::vector<std::vector<Grid>> mapGrid, int cellSize);
		LoggerInterface* logger;
	};

}
