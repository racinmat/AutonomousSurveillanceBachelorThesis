#include "Map.h"
#include "UavForRRT.h"

namespace App
{

	Map::Map()
	{
		obstacles = vector<shared_ptr<Obstacle>>();
		originalObstacles = vector<shared_ptr<Obstacle>>();
		goals = vector<shared_ptr<Goal>>();
		uavsStart = vector<shared_ptr<UavForRRT>>();
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

	void Map::addUavStart(shared_ptr<UavForRRT> start)
	{
		uavsStart.push_back(start);
	}

	vector<shared_ptr<UavForRRT>> Map::getUavsStart()
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

	mObject Map::toJson() const
	{
		mArray goals;
		for (auto goal : this->goals)
		{
			goals.push_back(goal->getRectangle()->toJson());
		}

		mArray obstacles;
		for (auto obstacle : this->originalObstacles)
		{
			obstacles.push_back(obstacle->rectangle->toJson());
		}
		mObject object;
		object["goals"] = goals;
		object["obstacles"] = obstacles;
		return object;
	}

	shared_ptr<Map> Map::fromJson(mValue data)
	{
		auto map = make_shared<Map>();
		auto goals = data.get_obj().at("goals");
		auto obstacles = data.get_obj().at("obstacles");
		for (auto goal : goals.get_array())
		{
			map->addGoal(Goal::fromJson(goal));
		}
		for (auto obstacle : obstacles.get_array())
		{
			map->addObstacle(Obstacle::fromJson(obstacle));
		}
		return map;
	}

	void Map::amplifyObstacles(int sizeIncrement)
	{
		for (auto obstacle : this->obstacles)
		{
			//hot bugfix so the obstacles wont overlap the map boundaries. The min and max height and width of the map should be taken from configuration.
			int minX = obstacle->rectangle->getX() - sizeIncrement;
			int minY = obstacle->rectangle->getY() - sizeIncrement;
			int width = obstacle->rectangle->getWidth() + 2 * sizeIncrement;
			int height = obstacle->rectangle->getHeight() + 2 * sizeIncrement;
			int maxX = minX + width;
			int maxY = minY + height;
			if (maxX > 1000)
			{
				width = width - (maxX - 1000);
			}
			if (maxY > 1000)
			{
				width = width - (maxY - 1000);
			}
			if (minX < 0)
			{
				width -= 0 - minX;
				minX = 0;
			}
			if (minY < 0)
			{
				height -= 0 - minY;
				minY = 0;
			}
			obstacle->rectangle->setX(minX);
			obstacle->rectangle->setY(minY);
			obstacle->rectangle->setWidth(width);
			obstacle->rectangle->setHeight(height);
		}
	}

	void Map::addObstacle(shared_ptr<Obstacle> obstacle)
	{
		obstacles.push_back(obstacle);
		originalObstacles.push_back(make_shared<Obstacle>(*obstacle.get()));
	}

}
