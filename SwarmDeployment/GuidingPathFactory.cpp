#include "GuidingPathFactory.h"

namespace App
{
	App::GuidingPathFactory::GuidingPathFactory()
	{
	}

	std::vector<App::Path*> GuidingPathFactory::createGuidingPaths(App::Map* map, int cellSize, int worldWidth, int worldHeigh)
	{
		Grid** mapGrid = getMapGrid(map, cellSize, worldWidth, worldHeigh);	//map object and parameters to 2D matrix of enums (grid)
		std::vector<Node*> nodes = gridToNodes(mapGrid);
		//here comes A* algorithm
		std::vector<App::Path*> paths = std::vector<App::Path*>(map->countGoals());
		return paths;
	}

	Grid** GuidingPathFactory::getMapGrid(App::Map* map, int cellSize, int worldWidth, int worldHeigh)
	{
		Grid** grid = new Grid*[];
		for (int i = 0; i < worldWidth; i+= cellSize)
		{
			grid[i] = new Grid[];
			for (int j = 0; j < worldHeigh; j+= cellSize)
			{
				grid[i][j] = analyzeCell(map, new Point(i > 0 ? i - cellSize : i, j > 0 ? j - cellSize : j), new Point(i, j));	//ternary operator checking borders of map
			}
		}
		return grid;
	}

	Grid GuidingPathFactory::analyzeCell(Map* map, Point* leftBottom, Point* rightUpper)
	{
		//here comes collision detection
		return Grid::Free;
	}

	std::vector<Node*> GuidingPathFactory::gridToNodes(Grid** mapGrid)
	{
		std::vector<Node*> nodes = std::vector<Node*>();
		//here comes transfer from 2D matrix to array of nodes.
		return nodes;
	}
}
