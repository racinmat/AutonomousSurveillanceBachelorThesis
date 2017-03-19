#include "MapProcessor.h"
#include "VCollide/ColDetect.h"
#include "memory"
#include <algorithm>
#include <cfloat>
#include "Configuration.h"

using namespace std;

namespace App
{

	MapProcessor::MapProcessor(shared_ptr<LoggerInterface> logger, shared_ptr<Configuration> configuration) : logger(logger), configuration(configuration)
	{
	}

	MapProcessor::~MapProcessor()
	{
	}

	shared_ptr<MapGraph> MapProcessor::mapToNodes(shared_ptr<Map> map)
	{
//		, int cellSize, int worldWidth, int worldHeigh, double uavSize, bool allowSwarmSplitting
		int cellSize = configuration->getAStarCellSize();
		bool allowSwarmSplitting = configuration->getAllowSwarmSplitting();
		//firstly we have to get map as 2D matrix, grid
		auto mapGrid = getMapGrid(map);	//map object and parameters to 2D matrix of enums (grid)
		logger->logMapGrid(mapGrid);
		//now we get nodes from this grid
		auto nodes = gridToNodes(mapGrid, cellSize);

		countDistancesToObstacles(nodes, cellSize);

		logger->logMapNodes(nodes);


		//now we determine starting and ending node.
		//Start node is node, where starts UAV in middle
		//End node is node in middle of goalGroup rectangle
		shared_ptr<Node> startNode = getStartNode(nodes, map, cellSize);
		auto endNodes = getEndNodes(nodes, map, cellSize, allowSwarmSplitting);

		shared_ptr<MapGraph> graph = make_shared<MapGraph>(nodes, startNode, endNodes);
		return graph;
	}

	void MapProcessor::setLogger(const shared_ptr<LoggerInterface> logger_interface)
	{
		logger = logger_interface;
	}

	vector<vector<Grid>> MapProcessor::getMapGrid(shared_ptr<Map> map)
	{
		int cellSize = configuration->getAStarCellSize();
		int worldWidth = configuration->getWorldWidth();
		int worldHeigh = configuration->getWorldHeight();
		int gridRow = 0;
		int rows = floor(double(worldWidth) / double(cellSize));	//zaokrouhluji dol�, abych nevzorkoval nedefinovanou ��st mapy
		int columns = floor(double(worldHeigh) / double(cellSize));	//rad�i o��znu kus mapy, ne� z mapy vyjet
		auto grid = vector<vector<Grid>>(rows);						
		for (int i = cellSize; i <= worldWidth; i += cellSize)
		{
			grid[gridRow] = vector<Grid>(columns);
			int gridColumn = 0;
			for (int j = cellSize; j <= worldHeigh; j += cellSize)
			{
				grid[gridRow][gridColumn] = analyzeCell(map, Point(i - cellSize, j - cellSize), Point(i, j));
				gridColumn++;
			}
			gridRow++;
		}
		return grid;
	}

	Grid MapProcessor::analyzeCell(shared_ptr<Map> map, Point leftBottom, Point rightUpper)
	{
		double uavSize = configuration->getUavSize();
		ColDetect colDetect;
		Rectangle2D cell = Rectangle2D(leftBottom.getX(), leftBottom.getY(), rightUpper.getX() - leftBottom.getX(), rightUpper.getY() - leftBottom.getY());

		for (auto uavStart : map->getUavsStart())
		{
			if (colDetect.coldetectWithoutTransformation(
				Rectangle2D(uavStart->getPointParticle()->getLocation()->getX() - uavSize / 2, uavStart->getPointParticle()->getLocation()->getY() - uavSize / 2, uavSize, uavSize), cell))
			{
				return Grid::UAV;
			}
		}

		for (auto obstacle : map->getObstacles())
		{
			if (colDetect.coldetectWithoutTransformation(
				Rectangle2D(obstacle->rectangle->getX(), obstacle->rectangle->getY(), obstacle->rectangle->getWidth(), obstacle->rectangle->getHeight()), cell))
			{
				return Grid::Obstacle;
			}
		}

		for (auto goal : map->getGoals())
		{
			if (colDetect.coldetectWithoutTransformation(
				Rectangle2D(goal->getRectangle()->getX(), goal->getRectangle()->getY(), goal->getRectangle()->getWidth(), goal->getRectangle()->getHeight()), cell))
			{
				return Grid::Goal;
			}
		}

		return Grid::Free;
	}

	vector<shared_ptr<Node>> MapProcessor::gridToNodes(vector<vector<Grid>> mapGrid, int cellSize)
	{
		double cost_neighbor = 30; // cena node, pokud je n�kde vedle n� p�ek�ka
		double cost_diagonal = 15; // cena node, pokud je diagon�ln� k n� p�ek�ka
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

		index = 0;	//index dan� node

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
						bool isOutOfMap = (i + p) < 0 || (j  + q) < 0 //kontrola 1. ��dku a 1. sloupce
							|| (i + p) >= mapGrid.size() || (j + q) >= row.size(); //kontrola posledn�ho ��dku a posledn�ho sloupce
						if (
							isNeighbor && !isOutOfMap
						) {
							auto neighbor = nodes[(i + p) * mapGrid.size() + (j + q)];
							if (neighbor)	//kontrola empty pointeru
							{
								if (neighbor->getGridType() == Grid::Obstacle)
								{
									if (p == 0 || q == 0)	//p��m� soused
									{
										node->increaseCost(cost_neighbor);
									}
									else	//soused na diagon�le
									{
										node->increaseCost(cost_diagonal);
									}
								}
								//sousedy p�id�v�m v�dy, kv�li ur�ov�n� vzd�le nosti od p�ek�ek. u a staru si sousedy filtruju a expanduji jen do soused� bez p�ek�ek
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

		//pokud za��naj� UAV na za��tku jinak nato�en� ne� je sm�r vedoc� cesty, mus� se slo�it� ot��et, proto vyberu ze v�ech nodes, na kter�ch le�� n�jak� uav, node, kter� le�� ve sm�ru pr�m�rn�ho oto�en� UAV
		//sm�r u��m podle �hlu, kter� dan� node sv�r� s pr�m�rn�m bodem mezi v�emi UAV.
		vector<shared_ptr<Node>> startingNodes = vector<shared_ptr<Node>>();
		for (auto node : nodes)
		{
			for (auto uav : map->getUavsStart())
			{
				if (node->contains(uav->getPointParticle()->getLocation()->getX(), uav->getPointParticle()->getLocation()->getY(), cellSize / 2))	//nalezen� node, ve kter� je st�ed
				{
					startingNodes.push_back(node);
					break;
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
			if (fabs(angle - averageRotation) < similarRotation)
			{
				similarRotation = fabs(angle - averageRotation);
				startNode = possibleStartNode;
			}
		}

		return startNode;
	}

	vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> MapProcessor::getEndNodes(vector<shared_ptr<Node>> nodes, shared_ptr<Map> map, int cellSize, bool allowSwarmSplitting)
	{
		auto placementMethod = configuration->getPlacementMethod();
		vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> endNodes;
		if (allowSwarmSplitting)
		{
			//end node pro ka�d� c�l zvl᚝
			endNodes = vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>>(map->getGoals().size());
			for (size_t i = 0; i < map->getGoals().size(); i++)
			{
				auto goal = map->getGoals()[i];
				auto middle = goal->getMiddle();

				auto node = findFirstNodeInRange(nodes, middle, cellSize / 2);
				endNodes[i] = make_tuple(node, goal);
			}
		} else
		{
			if (placementMethod == PlacementMethod::Standard)
			{
				//hled�m node, kter� je nejbl�e vypo��tan�mu st�edu a kde nen� p�ek�ka
				endNodes = vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>>(1);
				//end node je jen jedna, pro cel� goalGroup
				auto goalMiddle = map->getGoalGroup()->getMiddle();

				double range = -cellSize / 2;	//za�nu z�porn�, proto�e hned na za��tku p�i��t�m.
				shared_ptr<Node> node = nullptr;
				do		//pokud je v c�lov� node p�ek�ka, zv�t��m radius o 1 a hled�m d�l
				{
					range += cellSize;	//todo: contains metoda pracuje v range pro �tverec, mo�n� by bylo dobr� to p�ed�lat na kru�nici
					node = findFirstNodeInRange(nodes, goalMiddle, range);
				} while (!node);
				endNodes[0] = make_tuple(node, map->getGoalGroup());
			} else if(placementMethod == PlacementMethod::Chain)
			{
				//funguje pouze pro 2 AoI
				endNodes = vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>>(1);

				if (map->countGoals() != 2)
				{
					throw "PlacementMethod::Chain works only for 2 goals.";
				}

				//end node pro ka�d� c�l zvl᚝
				endNodes = vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>>(2);

				for (size_t i = 0; i < map->getGoals().size(); i++)
				{
					auto goal = map->getGoals()[i];
					auto middle = goal->getMiddle();

					auto node = findFirstNodeInRange(nodes, middle, cellSize / 2);
					endNodes[i] = make_tuple(node, goal);
				}
			}else
			{
				throw "Unknown placement method option.";
			}
		}

		return endNodes;
	}

	void MapProcessor::countDistancesToObstacles(vector<shared_ptr<Node>> nodes, int cellSize)
	{
		//nejd��ve za�nu od nodes s p�ek�kami. ty expanduji (pouze direct neighbors) a expandovan�m nod�m ulo��m vzd�lenost 1 a ulo��m sousedy do seznamu. P�vodn� nody ze seznamu odeb�r�m.
		//pak opakuji, zvy�uji vzd�lenost. neohodnocenou node pozn�m podle n�jak� defaultn� hodnoty.
		//pokud je ji� node ohodnocen�, ale na�el jsem ji s v�t�� vzd�lenost� od jin� p�ek�ky, p�ep�u v�t�� vzd�lenost men�� vzd�lenost�.
		//max. tolik expanz�, kolik je ���ka mapy. v�ce nen� pot�eba. 
		//nev�m, jestli expandovat zvl᚝ ka�dou p�ek�ku nebo najednou

		//todo: vyzkou�et, jak je to s push_back a pln� neobsazen�m vektorem		
		vector<shared_ptr<Node>> openedNodes = vector<shared_ptr<Node>>();
		for (auto node : nodes)
		{
			if (node->getGridType() == Grid::Obstacle)	//kontrola non empty node.
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
			} while (!node && !openedNodes.empty());	//kontrola null pointeru a nepr�zdn�ho pole
			if (!node)	//pole se vypr�zdnilo a tak je node null
			{
				break;
			}

			if (node->getGridType() == Grid::Obstacle)
			{
				node->setDistanceToObstacle(0);
			}
			//distance incremented, because neighbors have n+unitDistance distance from nearest obstacle, when node has distance n.
			double neighborsDistance = node->getDistanceToObstacle() + getDistanceBetweenNodes(cellSize);	
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

	//aby se vzd�lenost node od p�ek�ky nem�nila podle hustoty diskretizace (p�i 2x hust��ch �tverc�ch se bude vzd�lenost jevit jako 2x v�t��), p�epo��t� se vzd�lenost podle cellSize
	double MapProcessor::getDistanceBetweenNodes(int cellSize)
	{
		double reference = 20;
		return double(cellSize) / reference;
	}

	shared_ptr<Node> MapProcessor::findFirstNodeInRange(vector<shared_ptr<Node>>nodes, shared_ptr<Point> point, double range)
	{
		for (auto node : nodes)
		{
			if (node->getGridType() != Grid::Obstacle && node->contains(point, range))	//nalezen� node, ve kter� je st�ed
			{
				return node;
			}
		}
		return nullptr;
	}
}
