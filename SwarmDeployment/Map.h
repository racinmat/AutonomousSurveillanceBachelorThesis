#pragma once
#include "Goal.h"
#include "Obstacle.h"
#include <vector>
#include "PointParticle.h"

namespace App
{
	enum class Grid { Free, Obstacle, UAV, Goal };

	class Map
	{
	public:
		Map();
		void addObstacle(Obstacle* obstacle);
		void addGoal(Goal* goal);
		size_t countGoals() const;
		std::vector<Goal*> getGoals();
		std::vector<Obstacle*> getObstacles();
		void addUavStart(PointParticle* start);
		std::vector<PointParticle*> getUavsStart();

	protected:
		std::vector<Goal*> goals;
		std::vector<Obstacle*> obstacles;
		std::vector<PointParticle*> uavsStart;
	};

}
