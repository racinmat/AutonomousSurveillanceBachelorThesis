#include "MapProcessor.h"
#include "VCollide/ColDetect.h"

namespace App
{

	MapProcessor::MapProcessor(LoggerInterface* logger) : logger(logger)
	{
	}

	MapProcessor::~MapProcessor()
	{
	}

	MapGraph* MapProcessor::mapToNodes(App::Map* map, int cellSize, int worldWidth, int worldHeigh, double uavSize)
	{
		//firstly we have to get map as 2D matrix, grid
		auto mapGrid = getMapGrid(map, cellSize, worldWidth, worldHeigh, uavSize);	//map object and parameters to 2D matrix of enums (grid)
		logger->logMapGrid(mapGrid);
		//now we get nodes from this grid
		auto nodes = gridToNodes(mapGrid, cellSize);

		//now we determine starting and ending node.
		//Start node is node, where starts UAV in middle
		//End node is node in middle of each goal recrangle
		//Todo: vymyslet, zda zde natvrdo používat pro nalezení støedu obdélníky èi ne
		Node* startNode;

		int uavCount = map->countUavs();
		Point* middleUav = map->getUavsStart()[uavCount / 2]->getLocation();
		for (Node* node : nodes)
		{
			if (node->contains(middleUav->getX(), middleUav->getY(), cellSize / 2))	//nalezení node, ve které je støed
			{
				startNode = node;
			}
		}


		std::vector<Node*> endNodes = std::vector<Node*>(map->getGoals().size());
		for (size_t i = 0; i < map->getGoals().size(); i++)
		{
			Rectangle* goal = map->getGoals()[i]->rectangle;
			int middleX = goal->getX() + goal->getWidth() / 2;
			int middleY = goal->getY() + goal->getHeight() / 2;


			for(Node* node : nodes)
			{
				if (node->contains(middleX, middleY, cellSize / 2))	//nalezení node, ve které je støed
				{
					endNodes[i] = node;
				}
			}
		}

		MapGraph* graph = new MapGraph(nodes, startNode, endNodes);
		return graph;
	}

	std::vector<std::vector<Grid>> MapProcessor::getMapGrid(App::Map* map, int cellSize, int worldWidth, int worldHeigh, double uavSize)
	{
		int gridRow = 0;
		int rows = ceil(double(worldWidth) / double(cellSize));	//todo: zkontrolovat, zda nemusím pøièíst 1, podle zaokrouhlování
		int columns = ceil(double(worldHeigh) / double(cellSize));	//todo: zkontrolovat, zda nemusím pøièíst 1, podle zaokrouhlování
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
				nodes[i] = new Node(new Point(x, y), grid);

				index++;
			}
		}

		double cost_neighbor = 10; // cena node, pokud je nìkde vedle ní pøekážka
		double cost_diagonal = 5; // cena node, pokud je diagonálnì k ní pøekážka


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
						) {
							if (gridType == Grid::Obstacle)	//zvýšení ceny, pokud je soused pøekážka
							{
								if (p == 0 || q == 0)	//pøímý soused
								{
									nodes[i]->increaseCost(cost_neighbor);
								} else	//soused na diagonále
								{
									nodes[i]->increaseCost(cost_diagonal);
								}
							} else // nechci mezi sousedy pøekážky
							{
								nodes[i]->addNeighbor(nodes[(i + p) * mapGrid.size() + (j + q)], n_index);
								n_index++;
							}
						}
					}
				}
			}
		}

		return nodes;
	}


}
