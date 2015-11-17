#include "MapProcessor.h"
#include "VCollide/ColDetect.h"
#include "memory"

using namespace std;

namespace App
{

	MapProcessor::MapProcessor(shared_ptr<LoggerInterface> logger) : logger(logger)
	{
	}

	MapProcessor::~MapProcessor()
	{
	}

	shared_ptr<MapGraph> MapProcessor::mapToNodes(shared_ptr<Map> map, int cellSize, int worldWidth, int worldHeigh, double uavSize)
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
		shared_ptr<Node> startNode;

		int uavCount = map->countUavs();
		shared_ptr<Point> middleUav = map->getUavsStart()[uavCount / 2]->getLocation();
		for (auto node : nodes)
		{
			if (node->contains(middleUav->getX(), middleUav->getY(), cellSize / 2))	//nalezení node, ve které je støed
			{
				startNode = node;
				break;
			}
		}


		auto endNodes = vector<shared_ptr<Node>>(map->getGoals().size());
		for (size_t i = 0; i < map->getGoals().size(); i++)
		{
			auto goal = map->getGoals()[i]->rectangle;
			int middleX = goal->getX() + goal->getWidth() / 2;
			int middleY = goal->getY() + goal->getHeight() / 2;


			for(auto node : nodes)
			{
				if (node->contains(middleX, middleY, cellSize / 2))	//nalezení node, ve které je støed
				{
					endNodes[i] = node;
					break;
				}
			}
		}

		//úprava, aby mapa odpovídala matlabovské pøedloze pro pøesné porovnávání
		//nastaveno pro mapu 0
//		bool modifyByHand = true;
//		if (modifyByHand)
//		{
//			for (auto node : nodes)
//			{
//				if (node->contains(75, 75, cellSize / 2))	//nalezení node, ve které je støed
//				{
//					startNode = node;
//					break;
//				}
//			}
//
//			for (size_t i = 0; i < map->getGoals().size(); i++)
//			{
//
//				for (auto node : nodes)
//				{
//					if (node->contains(675, 775, cellSize / 2))	//nalezení node, ve které je støed
//					{
//						endNodes[i] = node;
//						break;
//					}
//				}
//			}
//		}


		shared_ptr<MapGraph> graph = make_shared<MapGraph>(nodes, startNode, endNodes);
		return graph;
	}

	vector<vector<Grid>> MapProcessor::getMapGrid(shared_ptr<Map> map, int cellSize, int worldWidth, int worldHeigh, double uavSize)
	{
		int gridRow = 0;
		int rows = ceil(double(worldWidth) / double(cellSize));	//todo: zkontrolovat, zda nemusím pøièíst 1, podle zaokrouhlování
		int columns = ceil(double(worldHeigh) / double(cellSize));	//todo: zkontrolovat, zda nemusím pøièíst 1, podle zaokrouhlování
		auto grid = vector<vector<Grid>>(rows);
		for (int i = cellSize; i <= worldWidth; i += cellSize)
		{
			grid[gridRow] = vector<Grid>(columns);
			int gridColumn = 0;
			for (int j = cellSize; j <= worldHeigh; j += cellSize)
			{
				grid[gridRow][gridColumn] = analyzeCell(map, Point(i - cellSize, j - cellSize), Point(i, j), uavSize);	//ternary operator checking borders of map
				gridColumn++;
			}
			gridRow++;
		}
		return grid;
	}

	Grid MapProcessor::analyzeCell(shared_ptr<Map> map, Point leftBottom, Point rightUpper, double uavSize)
	{
		ColDetect colDetect;
		Rectangle2D cell = Rectangle2D(leftBottom.getX(), leftBottom.getY(), rightUpper.getX() - leftBottom.getX(), rightUpper.getY() - leftBottom.getY());

		for (auto uavStart : map->getUavsStart())
		{
			if (colDetect.coldetect(
				Rectangle2D(uavStart->getLocation()->getX() - uavSize / 2, uavStart->getLocation()->getY() - uavSize / 2, uavSize, uavSize), cell))
			{
				return Grid::UAV;
			}
		}

		for (auto obstacle : map->getObstacles())
		{
			if (colDetect.coldetect(
				Rectangle2D(obstacle->rectangle->getX(), obstacle->rectangle->getY(), obstacle->rectangle->getWidth(), obstacle->rectangle->getHeight()), cell))
			{
				return Grid::Obstacle;
			}
		}

		for (auto goal : map->getGoals())
		{
			if (colDetect.coldetect(
				Rectangle2D(goal->rectangle->getX(), goal->rectangle->getY(), goal->rectangle->getWidth(), goal->rectangle->getHeight()), cell))
			{
				return Grid::Goal;
			}
		}

		return Grid::Free;
	}

	vector<shared_ptr<Node>> MapProcessor::gridToNodes(vector<vector<Grid>> mapGrid, int cellSize)
	{
		auto nodes = vector<shared_ptr<Node>>(mapGrid.size() * mapGrid[0].size());
		int index = 0;

		for (size_t i = 0; i < mapGrid.size(); i++)
		{
			auto row = mapGrid[i];
			for (size_t j = 0; j < row.size(); j++)
			{
				Grid grid = row[j];
				int x = i * cellSize + cellSize / 2;
				int y = j * cellSize + cellSize / 2;
				nodes[index] = make_shared<Node>(make_shared<Point>(x, y), grid);

				index++;
			}
		}

		double cost_neighbor = 10; // cena node, pokud je nìkde vedle ní pøekážka
		double cost_diagonal = 5; // cena node, pokud je diagonálnì k ní pøekážka
		index = 0;	//index dané node

		//adding of neighbors, when all nodes are added
		for (size_t i = 0; i < mapGrid.size(); i++)
		{
			auto row = mapGrid[i];
			for (size_t j = 0; j < row.size(); j++)
			{
				auto node = nodes[index];
				int n_index = 0;
				for (int p = -1; p <= 1; p++)
				{
					for (int q = -1; q <= 1; q++)
					{
						bool isNeighbor = p != 0 || q != 0;
						bool isOutOfMap = (i + p) < 0 || (j  + q) < 0 //kontrola 1. øádku a 1. sloupce
							|| (i + p) >= mapGrid.size() || (j + q) >= row.size(); //kontrola posledního øádku a posledního sloupce
						if (
							isNeighbor && !isOutOfMap
						) {
							if (node->getGridType() == Grid::Obstacle)	//zvýšení ceny, pokud je soused pøekážka
							{
								if (p == 0 || q == 0)	//pøímý soused
								{
									node->increaseCost(cost_neighbor);
								} else	//soused na diagonále
								{
									node->increaseCost(cost_diagonal);
								}
							} else // nechci mezi sousedy pøekážky
							{
								node->addNeighbor(nodes[(i + p) * mapGrid.size() + (j + q)], n_index);
								n_index++;
							}
						}
					}
				}
				index++;
			}
		}

		return nodes;
	}


}
