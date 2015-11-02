#include "GuidingPathFactory.h"
#include "VCollide/ColDetect.h"

namespace App
{

	GuidingPathFactory::GuidingPathFactory(LoggerInterface* logger) : logger(logger)
	{
	}

	std::vector<App::Path*> GuidingPathFactory::createGuidingPaths(App::Map* map, int cellSize, int worldWidth, int worldHeigh, double uavSize)
	{
		auto mapGrid = getMapGrid(map, cellSize, worldWidth, worldHeigh, uavSize);	//map object and parameters to 2D matrix of enums (grid)
		logger->logMapGrid(mapGrid);
		auto nodes = gridToNodes(mapGrid);
		//here comes A* algorithm
		auto paths = std::vector<App::Path*>(map->countGoals());
		return paths;
	}

	std::vector<std::vector<Grid>> GuidingPathFactory::getMapGrid(App::Map* map, int cellSize, int worldWidth, int worldHeigh, double uavSize)
	{
		int gridRow = 0;
		int rows = worldWidth / cellSize;	//todo: zkontrolovat, zda nemusím přičíst 1, podle zaokrouhlování
		int columns = worldHeigh / cellSize;	//todo: zkontrolovat, zda nemusím přičíst 1, podle zaokrouhlování
		auto grid = std::vector<std::vector<Grid>>(rows);
		for (int i = cellSize; i <= worldWidth; i+= cellSize)
		{
			grid[gridRow] = std::vector<Grid>(columns);
			int gridColumn = 0;
			for (int j = cellSize; j <= worldHeigh; j+= cellSize)
			{
				grid[gridRow][gridColumn] = analyzeCell(map, new Point(i - cellSize, j - cellSize), new Point(i, j), uavSize);	//ternary operator checking borders of map
				gridColumn++;
			}
			gridRow++;
		}
		return grid;
	}

	Grid GuidingPathFactory::analyzeCell(Map* map, Point* leftBottom, Point* rightUpper, double uavSize)
	{
		ColDetect colDetect;
		Rectangle2D* cell = new Rectangle2D(leftBottom->getX(), leftBottom->getY(), rightUpper->getX() - leftBottom->getX(), rightUpper->getY() - leftBottom->getY());

		for (PointParticle* uavStart : map->getUavsStart())
		{
			if (colDetect.coldetect(
				new Rectangle2D(uavStart->getLocation()->getX() - uavSize / 2, uavStart->getLocation()->getY() - uavSize / 2, uavSize, uavSize), cell))
			{
				return Grid::UAV;
			}
		}

		for (Obstacle* obstacle : map->getObstacles())
		{
			if(colDetect.coldetect(
				new Rectangle2D(obstacle->rectangle->getX(), obstacle->rectangle->getY(), obstacle->rectangle->getWidth(), obstacle->rectangle->getHeight()), cell))
			{
				return Grid::Obstacle;
			}
		}

		for (Goal* goal : map->getGoals())
		{
			if (colDetect.coldetect(
				new Rectangle2D(goal->rectangle->getX(), goal->rectangle->getY(), goal->rectangle->getWidth(), goal->rectangle->getHeight()), cell))
			{
				return Grid::Goal;
			}
		}

		return Grid::Free;
	}

	std::vector<Node*> GuidingPathFactory::gridToNodes(std::vector<std::vector<Grid>> mapGrid)
	{
		std::vector<Node*> nodes = std::vector<Node*>();
		//here comes transfer from 2D matrix to array of nodes.
		return nodes;
	}
}
