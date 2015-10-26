#include "../include/Map.h"
#include <vector>

namespace App
{

	Map::Map()
	{
		obstacles = std::vector<Obstacle*>();
		goals = std::vector<Goal*>();
	}

	void Map::addGoal(Goal *goal)
	{
		goals.push_back(goal);
	}

	int Map::countGoals() const
	{
		return goals.size;
	}

	void Map::addObstacle(Obstacle *obstacle)
	{
		obstacles.push_back(obstacle);
	}

}
