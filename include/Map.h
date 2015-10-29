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
		int countGoals() const;

	protected:
		std::vector<Goal*> goals;
		std::vector<Obstacle*> obstacles;
	};

}
