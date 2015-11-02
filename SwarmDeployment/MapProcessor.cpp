#include "MapProcessor.h"
#include "GuidingPathFactory.h"
#include "VCollide/ColDetect.h"

namespace App
{

	MapProcessor::MapProcessor(LoggerInterface* logger) : logger(logger)
	{
	}

	MapProcessor::~MapProcessor()
	{
	}

	std::vector<Node*> MapProcessor::mapToNodes(App::Map* map, int cellSize, int worldWidth, int worldHeigh, double uavSize)
	{
		auto mapGrid = getMapGrid(map, cellSize, worldWidth, worldHeigh, uavSize);	//map object and parameters to 2D matrix of enums (grid)
		logger->logMapGrid(mapGrid);
		auto nodes = gridToNodes(mapGrid, cellSize);
		return nodes;
	}

	std::vector<std::vector<Grid>> MapProcessor::getMapGrid(App::Map* map, int cellSize, int worldWidth, int worldHeigh, double uavSize)
	{
		int gridRow = 0;
		int rows = worldWidth / cellSize;	//todo: zkontrolovat, zda nemusím pøièíst 1, podle zaokrouhlování
		int columns = worldHeigh / cellSize;	//todo: zkontrolovat, zda nemusím pøièíst 1, podle zaokrouhlování
		auto grid = std::vector<std::vector<Grid>>(rows);
		for (int i = cellSize; i <= worldWidth; i += cellSize)
		{
			grid[gridRow] = std::vector<Grid>(columns);
			int gridColumn = 0;
			for (int j = cellSize; j <= worldHeigh; j += cellSize)
			{
				grid[gridRow][gridColumn] = analyzeCell(map, new Point(i - cellSize, j - cellSize), new Point(i, j), uavSize);	//ternary operator checking borders of map
				gridColumn++;
			}
			gridRow++;
		}
		return grid;
	}

	Grid MapProcessor::analyzeCell(Map* map, Point* leftBottom, Point* rightUpper, double uavSize)
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
			if (colDetect.coldetect(
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

	std::vector<Node*> MapProcessor::gridToNodes(std::vector<std::vector<Grid>> mapGrid, int cellSize)
	{
		std::vector<Node*> nodes = std::vector<Node*>(mapGrid.size() * mapGrid[0].size());
		int index = 0;

		for (size_t i = 0; i < mapGrid.size(); i++)
		{
			auto row = mapGrid[i];
			for (size_t j = 0; j < row.size(); j++)
			{
				Grid grid = row[j];
				int x = i * cellSize + cellSize / 2;
				int y = j * cellSize + cellSize / 2;
				nodes[i] = new Node(new Point(x, y));

				index++;
			}
		}

		//adding of neighbors, when all nodes are added
		for (size_t i = 0; i < mapGrid.size(); i++)
		{
			auto row = mapGrid[i];
			for (size_t j = 0; j < row.size(); j++)
			{
				Grid gridType = row[j];
				int n_index = 0;
				for (int p = -1; p <= 1; p++)
				{
					for (int q = -1; q <= 1; q++)
					{
						if (
							(p != 0 || q != 0)
							&& i > 0 && j > 0 //kontrola 1. øádku a 1. sloupce
							&& i < mapGrid.size() - 1 && j < row.size() - 1 //kontrola posledního øádku a posledního sloupce
							&& gridType != Grid::Obstacle
							) {
							nodes[i]->addNeighbor(nodes[(i + p) * mapGrid.size() + (j + q)], n_index);
							n_index++;
						}
					}
				}
			}
		}
		return nodes;
	}

}
