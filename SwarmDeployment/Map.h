#pragma once
#include "Goal.h"
#include "Obstacle.h"
#include <vector>
#include <memory>

using namespace std;

namespace App
{
	class Uav;

	class Map
	{
	public:
		Map();
		~Map();
		void addObstacle(shared_ptr<Obstacle> obstacle);
		void addGoal(shared_ptr<Goal> goal);
		size_t countGoals() const;
		std::vector<shared_ptr<Goal>> getGoals();
		std::vector<shared_ptr<Obstacle>> getObstacles();
		void addUavStart(shared_ptr<Uav> start);
		std::vector<shared_ptr<Uav>> getUavsStart();
		int countUavs() const;

	protected:
		std::vector<shared_ptr<Goal>> goals;
		std::vector<shared_ptr<Obstacle>> obstacles;
		std::vector<shared_ptr<Uav>> uavsStart;
	};

}
