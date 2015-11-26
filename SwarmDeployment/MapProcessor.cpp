#include "MapProcessor.h"
#include "VCollide/ColDetect.h"
#include "memory"
#include <algorithm>

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

		countDistancesToObstacles(nodes);

		logger->logMapNodes(nodes);


		//now we determine starting and ending node.
		//Start node is node, where starts UAV in middle
		//End node is node in middle of each goal recrangle
		//Todo: vymyslet, zda zde natvrdo pouívat pro nalezení støedu obdélníky èi ne
		//todo: vymyslet, jak pouít node s podobnım náklonem, pokud nejsou uav pøesnì na hranách ètvercù. moná brát ještì okolní nodes, pokud na nich není pøekáka?
		shared_ptr<Node> startNode = getStartNode(nodes, map, cellSize);
		auto endNodes = getEndNodes(nodes, map, cellSize);


		//úprava, aby mapa odpovídala matlabovské pøedloze pro pøesné porovnávání
		//nastaveno pro mapu 3
		bool modifyByHand = false;
		if (modifyByHand)
		{
			for (auto node : nodes)
			{
				if (node->contains(75, 75, cellSize / 2))	//nalezení node, ve které je støed
				{
					startNode = node;
					break;
				}
			}

			for (size_t i = 0; i < map->getGoals().size(); i++)
			{

				for (auto node : nodes)
				{
					if (node->contains(725, 775, cellSize / 2))	//nalezení node, ve které je støed
					{
						endNodes[i] = node;
						break;
					}
				}
			}
		}


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
				Rectangle2D(uavStart->getPointParticle()->getLocation()->getX() - uavSize / 2, uavStart->getPointParticle()->getLocation()->getY() - uavSize / 2, uavSize, uavSize), cell))
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

		double cost_neighbor = 10; // cena node, pokud je nìkde vedle ní pøekáka
		double cost_diagonal = 5; // cena node, pokud je diagonálnì k ní pøekáka
		index = 0;	//index dané node

		//adding of neighbors, when all nodes are added
		for (size_t i = 0; i < mapGrid.size(); i++)
		{
			auto row = mapGrid[i];
			for (size_t j = 0; j < row.size(); j++)
			{
				auto node = nodes[index];
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
							auto neighbor = nodes[(i + p) * mapGrid.size() + (j + q)];
							if (neighbor)	//kontrola empty pointeru
							{
								if (neighbor->getGridType() == Grid::Obstacle)
								{
									if (p == 0 || q == 0)	//pøímı soused
									{
										node->increaseCost(cost_neighbor);
									}
									else	//soused na diagonále
									{
										node->increaseCost(cost_diagonal);
									}
								}
								//sousedy pøidávám vdy, kvùli urèování vzdále nosti od pøekáek. u a staru si sousedy filtruju a expanduji jen do sousedù bez pøekáek
								bool isDiagonal = p != 0 && q != 0;
								node->addNeighbor(neighbor, isDiagonal);
							}
						}
					}
				}
				index++;
			}
		}

		return nodes;
	}

	shared_ptr<Node> MapProcessor::getStartNode(vector<shared_ptr<Node>> nodes, shared_ptr<Map> map, int cellSize)
	{
		shared_ptr<Node> startNode;
		int uavCount = map->countUavs();
//		shared_ptr<Point> middleUav = map->getUavsStart()[uavCount / 2]->getPointParticle()->getLocation();
//		for (auto node : nodes)
//		{
//			if (node->contains(middleUav->getX(), middleUav->getY(), cellSize / 2))	//nalezení node, ve které je støed
//			{
//				startNode = node;
//				break;
//			}
//		}


		//pokud zaèínají UAV na zaèátku jinak natoèená ne je smìr vedocí cesty, musí se sloitì otáèet, proto vyberu ze všech nodes, na kterıch leí nìjaké uav, node, která leí ve smìru prùmìrného otoèení UAV
		//smìr uèím podle úhlu, kterı daná node svírá s prùmìrnım bodem mezi všemi UAV.
		vector<shared_ptr<Node>> startingNodes = vector<shared_ptr<Node>>();
		for (auto node : nodes)
		{
			//todo: zjistit, proè pøi mapì 6 a aStarCellSize = 9 se objevují empty prvky
			if(node)
			{
				for (auto uav : map->getUavsStart())
				{
					if (node->contains(uav->getPointParticle()->getLocation()->getX(), uav->getPointParticle()->getLocation()->getY(), cellSize / 2))	//nalezení node, ve které je støed
					{
						startingNodes.push_back(node);
						break;
					}
				}
			}
		}

		double averageRotation = 0;
		shared_ptr<Point> averageLocation = make_shared<Point>(0, 0);
		for (auto uav : map->getUavsStart())
		{
			averageRotation += uav->getPointParticle()->getRotation()->getZ();	//u 2D je pouze rotace okolo osy Z
			averageLocation->changeX(uav->getPointParticle()->getLocation()->getX());
			averageLocation->changeY(uav->getPointParticle()->getLocation()->getY());
		}

		averageRotation /= uavCount;
		averageLocation->setX(averageLocation->getX() / uavCount);
		averageLocation->setY(averageLocation->getY() / uavCount);

		double similarRotation = DBL_MAX;
		for(auto possibleStartNode : startingNodes)
		{
			double angle = atan2(possibleStartNode->getPoint()->getY() - averageLocation->getY(), possibleStartNode->getPoint()->getX() - averageLocation->getX());
			if (abs(angle - averageRotation) < similarRotation)
			{
				similarRotation = abs(angle - averageRotation);
				startNode = possibleStartNode;
			}
		}

		return startNode;
	}

	vector<shared_ptr<Node>> MapProcessor::getEndNodes(vector<shared_ptr<Node>> nodes, shared_ptr<Map> map, int cellSize)
	{
		auto endNodes = vector<shared_ptr<Node>>(map->getGoals().size());
		for (size_t i = 0; i < map->getGoals().size(); i++)
		{
			auto goal = map->getGoals()[i]->rectangle;
			int middleX = goal->getX() + goal->getWidth() / 2;
			int middleY = goal->getY() + goal->getHeight() / 2;


			for (auto node : nodes)
			{
				if (node->contains(middleX, middleY, cellSize / 2))	//nalezení node, ve které je støed
				{
					endNodes[i] = node;
					break;
				}
			}
		}
		return endNodes;
	}

	void MapProcessor::countDistancesToObstacles(vector<shared_ptr<Node>> nodes)
	{
		//nejdøíve zaènu od nodes s pøekákami. ty expanduji (pouze direct neighbors) a expandovanım nodám uloím vzdálenost 1 a uloím sousedy do seznamu. Pùvodní nody ze seznamu odebírám.
		//pak opakuji, zvyšuji vzdálenost. neohodnocenou node poznám podle nìjaké defaultní hodnoty.
		//pokud je ji node ohodnocená, ale našel jsem ji s vìtší vzdáleností od jiné pøekáky, pøepíšu vìtší vzdálenost menší vzdáleností.
		//max. tolik expanzí, kolik je šíøka mapy. více není potøeba. 
		//nevím, jestli expandovat zvláš kadou pøekáku nebo najednou

		//todo: vyzkoušet, jak je to s push_back a plnì neobsazenım vektorem		
		vector<shared_ptr<Node>> openedNodes = vector<shared_ptr<Node>>();
		for (auto node : nodes)
		{
			if (node->getGridType() == Grid::Obstacle)
			{
				openedNodes.push_back(node);
			}
		}

		while (!openedNodes.empty())
		{
			shared_ptr<Node> node;
			do
			{
				node = openedNodes[0];
				openedNodes.erase(remove(openedNodes.begin(), openedNodes.end(), node));	//erase remove idiom. for removing. really, c++? really?
			} while (!node && !openedNodes.empty());	//kontrola null pointeru a neprázdného pole
			if (!node)	//pole se vyprázdnilo a tak je node null
			{
				break;
			}

			if (node->getGridType() == Grid::Obstacle)
			{
				node->setDistanceToObstacle(0);
			}
			int neighborsDistance = node->getDistanceToObstacle() + 1;	//distance incremented by one, because neighbors have n+1 distance from nearest obstacle, when node has distance n.
			auto neighbors = node->getDirectNeighbors();
			for (auto neighbor : neighbors)
			{

				if (neighbor->getDistanceToObstacle() > neighborsDistance)
				{
					neighbor->setDistanceToObstacle(neighborsDistance);
					openedNodes.push_back(neighbor);
				}
			}
		}
	}
}
