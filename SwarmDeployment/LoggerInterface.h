#pragma once
#include "Map.h"
#include "Path.h"
#include "memory"
#include "State.h"

namespace App
{

	class LoggerInterface
	{
	public:
		LoggerInterface();
		virtual ~LoggerInterface();
		virtual void logSelectedMap(shared_ptr<Map> map, int worldWidth, int worldHeight);
		virtual void logMapGrid(vector<vector<Grid>> mapGrid);
		virtual void logGuidingPaths(vector<shared_ptr<Path>> paths, shared_ptr<Node> start, vector<shared_ptr<Node>> ends);
		virtual void logText(string string);
		virtual void logNearNode(shared_ptr<State> nearNode);
		virtual void logNewNode(shared_ptr<State> newNode);
		virtual void logRandomStates(vector<shared_ptr<Point>> randomStates);
	};

}