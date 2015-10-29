#pragma once
#include "Path.h"
#include "Map.h"

namespace App
{

	class GuidingPathFactory
	{
	public:
		GuidingPathFactory();
		std::vector<App::Path*> createGuidingPaths(App::Map* map, int cellSize, int worldWidth, int worldHeigh);
	
	protected:
		Grid** getMapGrid(App::Map* map, int cellSize, int worldWidth, int worldHeigh);	//returns 2D matrix as grid of map
		Grid analyzeCell(Map* map, Point* leftBottom, Point* rightUpper);
		std::vector<Node*> gridToNodes(Grid** mapGrid);
	};

}
