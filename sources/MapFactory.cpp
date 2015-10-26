#include "../include/MapFactory.h"

namespace App
{

	std::vector<App::Map*> MapFactory::createMaps()
	{
		std::vector<Map*> maps = std::vector<Map*>(6);

		Map* map0 = new Map();
		map0->addGoal(new Goal(651, 701, 348, 148));
		map0->addObstacle(new Obstacle(0, 400, 700, 50));
		map0->addObstacle(new Obstacle(250, 600, 750, 50));
		map0->addObstacle(new Obstacle(400, 0, 50, 200));
		map0->addObstacle(new Obstacle(200, 600, 50, 200));
		map0->addObstacle(new Obstacle(650, 200, 50, 200));
		maps.push_back(map0);

		Map* map1 = new Map();
		map1->addGoal(new Goal(1, 426, 149, 148));
		map1->addGoal(new Goal(851, 426, 149, 148));
		map1->addObstacle(new Obstacle(151, 299, 99, 401));
		map1->addObstacle(new Obstacle(751, 299, 99, 401));
		map1->addObstacle(new Obstacle(451, 599, 99, 401));
		map1->addObstacle(new Obstacle(451, 0, 99, 401));
		maps.push_back(map1);

		Map* map2 = new Map();
		map2->addGoal(new Goal(651, 850, 230, 100));
		map2->addGoal(new Goal(651, 550, 230, 100));
		map2->addObstacle(new Obstacle(600, 650, 350, 200));
		map2->addObstacle(new Obstacle(49, 600, 400, 200));
		map2->addObstacle(new Obstacle(200, 100, 200, 400));
		map2->addObstacle(new Obstacle(100, 400, 100, 100));
		map2->addObstacle(new Obstacle(400, 400, 100, 100));
		map2->addObstacle(new Obstacle(600, 100, 300, 100));
		map2->addObstacle(new Obstacle(700, 200, 200, 299));
		maps.push_back(map2);

		Map* map3 = new Map();
		map3->addGoal(new Goal(701, 701, 150, 150));
		map3->addObstacle(new Obstacle(300, 300, 200, 400));
		maps.push_back(map3);

		Map* map4 = new Map();
		map4->addGoal(new Goal(301, 500, 398, 400));
		map4->addObstacle(new Obstacle(300, 300, 400, 200));
		maps.push_back(map4);

		Map* map5 = new Map();
		map5->addGoal(new Goal(385, 801, 230, 100));
		map5->addGoal(new Goal(301, 521, 150, 180));
		map5->addGoal(new Goal(551, 521, 150, 180));
		map5->addObstacle(new Obstacle(199, 699, 601, 1010));
		map5->addObstacle(new Obstacle(199, 519, 101, 201));
		map5->addObstacle(new Obstacle(699, 519, 101, 201));
		map5->addObstacle(new Obstacle(199, 199, 601, 101));
		maps.push_back(map5);

		return maps;
	}

}