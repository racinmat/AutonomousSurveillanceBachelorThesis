#include "Goal.h"
#include "Obstacle.h"
#include <vector>

class Map
{
public:
	Map();
	void addObstacle(Obstacle* obstacle);
	void addGoal(Goal* goal);

protected:
	std::vector<Goal*> goals;
	std::vector<Obstacle*> obstacles;
};
