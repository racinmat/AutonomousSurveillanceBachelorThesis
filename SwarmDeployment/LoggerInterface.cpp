#include "LoggerInterface.h"
#include "Path.h"

namespace App
{

	LoggerInterface::LoggerInterface()
	{
	}


	LoggerInterface::~LoggerInterface()
	{
	}

	void LoggerInterface::logSelectedMap(std::shared_ptr<Map> map, int worldWidth, int worldHeight)
	{
	}

	void LoggerInterface::logMapGrid(std::vector<std::vector<Grid>> mapGrid)
	{
	}

	void LoggerInterface::logMapNodes(vector<shared_ptr<Node>> nodes)
	{
	}

	void LoggerInterface::logAStarNode(shared_ptr<AStar::NodeWrapper> node)
	{
	}

	void LoggerInterface::logGuidingPaths(std::vector<std::shared_ptr<Path>> paths, std::shared_ptr<Node> start, vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> ends)
	{
	}

	void LoggerInterface::logText(std::string string)
	{
	}

	void LoggerInterface::logText(char const string[])
	{
	}

	void LoggerInterface::logNewState(shared_ptr<LinkedState> nearNode, shared_ptr<LinkedState> newNode, bool optimization)
	{
	}

	void LoggerInterface::logNewState(shared_ptr<State> nearNode, shared_ptr<State> newNode, bool optimization)
	{
	}

	void LoggerInterface::logRandomStates(unordered_map<UavForRRT, shared_ptr<Point>, UavHasher> randomStates)
	{
	}

	void LoggerInterface::logRandomStatesCenter(shared_ptr<Point> center)
	{
	}

	void LoggerInterface::logBestPath(vector<shared_ptr<State>> path, bool optimization)
	{
	}

	void LoggerInterface::logDubinsPaths(unordered_map<UavForRRT, pair<geom::Dubins, bool>, UavHasher> dubinsPaths)
	{
	}

	void LoggerInterface::saveVisualMap()
	{
	}
}
