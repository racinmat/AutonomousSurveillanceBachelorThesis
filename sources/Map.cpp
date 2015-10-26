#include "../include/Map.h"

Map::Map()
{
	obstacles = std::vector<Obstacle>();
	goals = std::vector<Goal>();
}

void Map::addGoal(Goal *goal)
{
	goals.push_back(goal);
}

void Map::addObstacle(Obstacle *obstacle)
{
	obstacles.push_back(obstacle);
}
