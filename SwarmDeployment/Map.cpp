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

	Value Map::toJson(Document::AllocatorType& allocator) const
	{
		Value object(kObjectType);
		Value goals(kArrayType);
		Value obstacles(kArrayType);

		for (auto goal : this->goals)
		{
			goals.PushBack(goal->getRectangle()->toJson(allocator), allocator);
		}

		for (auto obstacle : this->originalObstacles)
		{
			obstacles.PushBack(obstacle->rectangle->toJson(allocator), allocator);
		}

		object.AddMember("goals", goals, allocator);
		object.AddMember("obstacles", obstacles, allocator);
		return object;
	}

	shared_ptr<Map> Map::fromJson(Value& data)
	{
		auto map = make_shared<Map>();
		auto &goals = data["goals"];
		auto &obstacles = data["obstacles"];
		for (auto &goal : goals.GetArray())
		{
			map->addGoal(Goal::fromJson(goal));
		}
		for (auto &obstacle : obstacles.GetArray())
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
