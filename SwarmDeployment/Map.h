#pragma once
#include "Goal.h"
#include "Obstacle.h"
#include <vector>

namespace App
{

	class Map
	{
	public:
		Map();
		void addObstacle(Obstacle* obstacle);
		void addGoal(Goal* goal);
		size_t countGoals() const;
		std::vector<Goal*> getGoals();
		std::vector<Obstacle*> getObstacles();

	protected:
		std::vector<Goal*> goals;
		std::vector<Obstacle*> obstacles;
	};

}
