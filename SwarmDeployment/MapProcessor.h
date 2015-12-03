#pragma once
#include "Path.h"
#include "Map.h"
#include "LoggerInterface.h"
#include "MapGraph.h"

using namespace std;

namespace App
{
	class MapProcessor
	{
	public:
		MapProcessor(shared_ptr<LoggerInterface> logger);
		virtual ~MapProcessor();
		shared_ptr<MapGraph> mapToNodes(shared_ptr<Map> map, int cellSize, int worldWidth, int worldHeigh, double uavSize, bool allowSwarmSplitting);

	protected:
		vector<vector<Grid>> getMapGrid(shared_ptr<Map> map, int cellSize, int worldWidth, int worldHeigh, double uavSize);	//returns 2D matrix as grid of map
		Grid analyzeCell(shared_ptr<Map> map, Point leftBottom, Point rightUpper, double uavSize);
		vector<shared_ptr<Node>> gridToNodes(vector<vector<Grid>> mapGrid, int cellSize);
		shared_ptr<Node> getStartNode(vector<shared_ptr<Node>> nodes, shared_ptr<Map> map, int cellSize);
		vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> getEndNodes(vector<shared_ptr<Node>> nodes, shared_ptr<Map> map, int cellSize, bool allowSwarmSplitting);
		void countDistancesToObstacles(vector<shared_ptr<Node>> nodes, int cellSize);
		shared_ptr<LoggerInterface> logger;
		double getDistanceBetweenNodes(int cellSize);
	};

}
