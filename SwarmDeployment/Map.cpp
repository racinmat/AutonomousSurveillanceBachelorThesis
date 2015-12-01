#include "Map.h"
#include "Uav.h"

namespace App
{

	Map::Map()
	{
		obstacles = vector<shared_ptr<Obstacle>>();
		goals = vector<shared_ptr<Goal>>();
		uavsStart = vector<shared_ptr<Uav>>();
		goalGroup = make_shared<GoalGroup>();
	}

	Map::~Map()
	{
	}

	void Map::addGoal(shared_ptr<Goal> goal)
	{
		goals.push_back(goal);
		goalGroup->addGoal(goal);
	}

	size_t Map::countGoals() const
	{
		return goals.size();
	}

	vector<shared_ptr<Goal>> Map::getGoals()
	{
		return goals;
	}

	vector<shared_ptr<Obstacle>> Map::getObstacles()
	{
		return obstacles;
	}

	void Map::addUavStart(shared_ptr<Uav> start)
	{
		uavsStart.push_back(start);
	}

	vector<shared_ptr<Uav>> Map::getUavsStart()
	{
		return uavsStart;
	}

	int Map::countUavs() const
	{
		return uavsStart.size();
	}

	shared_ptr<GoalGroup> Map::getGoalGroup() const
	{
		return goalGroup;
	}

	void Map::addObstacle(shared_ptr<Obstacle> obstacle)
	{
		obstacles.push_back(obstacle);
	}

}
