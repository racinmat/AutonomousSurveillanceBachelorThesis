#include "Map.h"
#include "Uav.h"

namespace App
{

	Map::Map()
	{
		obstacles = vector<shared_ptr<Obstacle>>();
		originalObstacles = vector<shared_ptr<Obstacle>>();
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

	shared_ptr<Map> Map::fromJson(mObject json)
	{
		auto map = make_shared<Map>();
		//TODO: dodělat
		//		for(auto goalData : json.at("goals"))
		{
//			auto goal = Goal::
//			map->addGoal(goal);
		}
		return map;
	}

	void Map::amplifyObstacles(int sizeIncrement)
	{
		for (auto obstacle : this->obstacles)
		{
			obstacle->rectangle->setX(obstacle->rectangle->getX() - sizeIncrement);
			obstacle->rectangle->setY(obstacle->rectangle->getY() - sizeIncrement);
			obstacle->rectangle->setWidth(obstacle->rectangle->getWidth() + 2 * sizeIncrement);
			obstacle->rectangle->setHeight(obstacle->rectangle->getHeight() + 2 * sizeIncrement);
		}
	}

	void Map::addObstacle(shared_ptr<Obstacle> obstacle)
	{
		obstacles.push_back(obstacle);
		originalObstacles.push_back(make_shared<Obstacle>(*obstacle.get()));
	}

}
