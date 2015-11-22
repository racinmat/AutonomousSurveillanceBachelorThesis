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

	void LoggerInterface::logGuidingPaths(std::vector<std::shared_ptr<Path>> paths, std::shared_ptr<Node> start, std::vector<std::shared_ptr<Node>> ends)
	{
	}

	void LoggerInterface::logText(std::string string)
	{
	}

	void LoggerInterface::logNewState(shared_ptr<State> nearNode, shared_ptr<State> newNode)
	{
	}

	void LoggerInterface::logRandomStates(unordered_map<Uav, shared_ptr<Point>, UavHasher> randomStates)
	{
	}

	void LoggerInterface::logRandomStatesCenter(shared_ptr<Point> center)
	{
	}
}
