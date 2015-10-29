#include "Map.h"

namespace App
{

	Map::Map()
	{
		obstacles = std::vector<Obstacle*>();
		goals = std::vector<Goal*>();
		uavsStart = std::vector<PointParticle*>();
	}

	void Map::addGoal(Goal *goal)
	{
		goals.push_back(goal);
	}

	size_t Map::countGoals() const
	{
		return goals.size();
	}

	std::vector<Goal*> Map::getGoals()
	{
		return goals;
	}

	std::vector<Obstacle*> Map::getObstacles()
	{
		return obstacles;
	}

	void Map::addUavStart(PointParticle* start)
	{
		uavsStart.push_back(start);
	}

	std::vector<PointParticle*> Map::getUavsStart()
	{
		return uavsStart;
	}

	void Map::addObstacle(Obstacle *obstacle)
	{
		obstacles.push_back(obstacle);
	}

}
