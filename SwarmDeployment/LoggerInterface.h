#pragma once
#include "Map.h"
#include "Path.h"
#include "memory"
#include "State.h"
#include "Enums.h"
#include <unordered_map>
#include "NodeWrapper.h"
#include "Dubins/geom/Dubins.h"

namespace App
{

	class LoggerInterface
	{
	public:
		LoggerInterface();
		virtual ~LoggerInterface();
		virtual void logSelectedMap(shared_ptr<Map> map, int worldWidth, int worldHeight);
		virtual void logMapGrid(vector<vector<Grid>> mapGrid);
		virtual void logMapNodes(vector<shared_ptr<Node>> nodes);
		virtual void logAStarNode(shared_ptr<AStar::NodeWrapper> node);
		virtual void logGuidingPaths(vector<shared_ptr<Path>> paths, shared_ptr<Node> start, vector<tuple<shared_ptr<Node>, shared_ptr<GoalInterface>>> ends);
		virtual void logText(string string);
		virtual void logText(char const string[]);
		virtual void logNewState(shared_ptr<LinkedState> nearNode, shared_ptr<LinkedState> newNode, bool optimization = false);
		virtual void logRandomStates(unordered_map<Uav, shared_ptr<Point>, UavHasher> randomStates);
		virtual void logRandomStatesCenter(shared_ptr<Point> center);
		virtual void logBestPath(vector<shared_ptr<State>> path, bool optimization = false);
		virtual void logDubinsPaths(unordered_map<Uav, pair<geom::Dubins, bool>, UavHasher> dubinsPaths);
	};

}
