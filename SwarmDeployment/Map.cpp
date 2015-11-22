#include "Map.h"
#include "Uav.h"

namespace App
{

	Map::Map()
	{
		obstacles = std::vector<shared_ptr<Obstacle>>();
		goals = std::vector<shared_ptr<Goal>>();
		uavsStart = std::vector<shared_ptr<Uav>>();
	}

	Map::~Map()
	{
	}

	void Map::addGoal(shared_ptr<Goal> goal)
	{
		goals.push_back(goal);
	}

	size_t Map::countGoals() const
	{
		return goals.size();
	}

	std::vector<shared_ptr<Goal>> Map::getGoals()
	{
		return goals;
	}

	std::vector<shared_ptr<Obstacle>> Map::getObstacles()
	{
		return obstacles;
	}

	void Map::addUavStart(shared_ptr<Uav> start)
	{
		uavsStart.push_back(start);
	}

	std::vector<shared_ptr<Uav>> Map::getUavsStart()
	{
		return uavsStart;
	}

	int Map::countUavs() const
	{
		return uavsStart.size();
	}

	void Map::addObstacle(shared_ptr<Obstacle> obstacle)
	{
		obstacles.push_back(obstacle);
	}

}
