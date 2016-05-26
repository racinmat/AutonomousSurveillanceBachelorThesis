#pragma once
#include "Path.h"
#include "Map.h"
#include "LoggerInterface.h"
#include "MapGraph.h"

using namespace std;

namespace App
{
	class Configuration;

	class MapProcessor
	{
	public:
		MapProcessor(shared_ptr<LoggerInterface> logger, shared_ptr<Configuration> configuration);
		virtual ~MapProcessor();
		shared_ptr<MapGraph> mapToNodes(shared_ptr<Map> map);
		virtual void setLogger(const shared_ptr<LoggerInterface> logger_interface);

	protected:
		vector<vector<Grid>> getMapGrid(shared_ptr<Map> map);	//returns 2D matrix as grid of map
		Grid analyzeCell(shared_ptr<Map> map, Point leftBottom, Point rightUpper);
		vector<shared_ptr<Node>> gridToNodes(vector<vector<Grid>> mapGrid, int cellSize);
		shared_ptr<Node> getStartNode(vector<shared_ptr<Node>> nodes, shared_ptr<Map> map, int cellSize);
		vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> getEndNodes(vector<shared_ptr<Node>> nodes, shared_ptr<Map> map, int cellSize, bool allowSwarmSplitting);
		void countDistancesToObstacles(vector<shared_ptr<Node>> nodes, int cellSize);
		shared_ptr<LoggerInterface> logger;
		shared_ptr<Configuration> configuration;
		double getDistanceBetweenNodes(int cellSize);
		shared_ptr<Node> findFirstNodeInRange(vector<shared_ptr<Node>>nodes, shared_ptr<Point> point, double range);
	};

}
