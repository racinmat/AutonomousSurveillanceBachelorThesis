#include "MapFactory.h"

namespace App
{

	std::vector<App::Map*> MapFactory::createMaps(int uavCount)
	{
		int uavDistance = 30;

		std::vector<Map*> maps = std::vector<Map*>();

		Map* map0 = new Map();
		for (int i = 0; i < uavCount; i++)
		{
			map0->addUavStart(new PointParticle(50 + i * uavDistance, 50, 0, 0));
		}
		map0->addGoal(new Goal(651, 701, 348, 148));
		map0->addObstacle(new Obstacle(0, 400, 700, 50));
		map0->addObstacle(new Obstacle(250, 600, 750, 50));
		map0->addObstacle(new Obstacle(400, 0, 50, 200));
		map0->addObstacle(new Obstacle(200, 600, 50, 200));
		map0->addObstacle(new Obstacle(650, 200, 50, 200));
		maps.push_back(map0);

		Map* map1 = new Map();
		for (int i = 0; i < uavCount; i++)
		{
			map0->addUavStart(new PointParticle(401 + i * uavDistance, 501, 0, 0));
		}
		map1->addGoal(new Goal(1, 426, 149, 148));
		map1->addGoal(new Goal(851, 426, 149, 148));
		map1->addObstacle(new Obstacle(151, 299, 99, 401));
		map1->addObstacle(new Obstacle(751, 299, 99, 401));
		map1->addObstacle(new Obstacle(451, 599, 99, 401));
		map1->addObstacle(new Obstacle(451, 0, 99, 401));
		maps.push_back(map1);

		Map* map2 = new Map();
		for (int i = 0; i < uavCount; i++)
		{
			map0->addUavStart(new PointParticle(50 + i * uavDistance, 50, 0, 0));
		}
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
		for (int i = 0; i < uavCount; i++)
		{
			map0->addUavStart(new PointParticle(50 + i * uavDistance, 50, 0, 0));
		}
		map3->addGoal(new Goal(701, 701, 150, 150));
		map3->addObstacle(new Obstacle(300, 300, 200, 400));
		maps.push_back(map3);

		Map* map4 = new Map();
		for (int i = 0; i < uavCount; i++)
		{
			map0->addUavStart(new PointParticle(380 + i * uavDistance, 50, 0, 0));
		}
		map4->addGoal(new Goal(301, 500, 398, 400));
		map4->addObstacle(new Obstacle(300, 300, 400, 200));
		maps.push_back(map4);

		Map* map5 = new Map();
		for (int i = 0; i < uavCount; i++)
		{
			map0->addUavStart(new PointParticle(400 + i * uavDistance, 50, 0, 0));
		}
		map5->addGoal(new Goal(385, 801, 230, 100));
		map5->addGoal(new Goal(301, 521, 150, 180));
		map5->addGoal(new Goal(551, 521, 150, 180));
		map5->addObstacle(new Obstacle(199, 699, 601, 101));
		map5->addObstacle(new Obstacle(199, 519, 101, 201));
		map5->addObstacle(new Obstacle(699, 519, 101, 201));
		map5->addObstacle(new Obstacle(199, 199, 601, 101));
		maps.push_back(map5);

		Map* map6 = new Map();
		for (int i = 0; i < uavCount; i++)
		{
			map0->addUavStart(new PointParticle(400 + i * uavDistance, 50, 0, 0));
		}
		map6->addGoal(new Goal(701, 701, 150, 150));
		map6->addObstacle(new Obstacle(400, 0, 200, 470));
		map6->addObstacle(new Obstacle(400, 480, 200, 520));
		maps.push_back(map6);

		return maps;
	}

}