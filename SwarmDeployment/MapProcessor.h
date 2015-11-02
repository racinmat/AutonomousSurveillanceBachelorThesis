#pragma once
#include "Path.h"
#include "Map.h"
#include "LoggerInterface.h"

namespace App
{

	class MapProcessor
	{
	public:
		MapProcessor(LoggerInterface* logger);
		virtual ~MapProcessor();
		std::vector<Node*> mapToNodes(App::Map* map, int cellSize, int worldWidth, int worldHeigh, double uavSize);

	protected:
		std::vector<std::vector<Grid>> getMapGrid(App::Map* map, int cellSize, int worldWidth, int worldHeigh, double uavSize);	//returns 2D matrix as grid of map
		Grid analyzeCell(Map* map, Point* leftBottom, Point* rightUpper, double uavSize);
		std::vector<Node*> gridToNodes(std::vector<std::vector<Grid>> mapGrid, int cellSize);
		LoggerInterface* logger;
	};

}
