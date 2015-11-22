#include "MapFactory.h"
#include <memory>
#include "Uav.h"

#define PI 3.14159265358979323846

namespace App
{

	std::vector<std::shared_ptr<Map>> MapFactory::createMaps(int uavCount)
	{
		int uavDistance = 30;

		auto maps = std::vector<std::shared_ptr<Map>>();

		std::shared_ptr<Map> map0 = std::make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map0->addUavStart(make_shared<Uav>(80 + i * uavDistance, 50, PI/2));
		}
		map0->addGoal(make_shared<Goal>(651, 701, 348, 148));
		map0->addObstacle(make_shared<Obstacle>(0, 400, 700, 50));
		map0->addObstacle(make_shared<Obstacle>(250, 600, 750, 50));
		map0->addObstacle(make_shared<Obstacle>(400, 0, 50, 200));
		map0->addObstacle(make_shared<Obstacle>(200, 600, 50, 200));
		map0->addObstacle(make_shared<Obstacle>(650, 200, 50, 200));
		maps.push_back(map0);

		std::shared_ptr<Map> map1 = std::make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map1->addUavStart(make_shared<Uav>(431 + i * uavDistance, 501, PI / 2));
		}
		map1->addGoal(make_shared<Goal>(1, 426, 149, 148));
		map1->addGoal(make_shared<Goal>(851, 426, 149, 148));
		map1->addObstacle(make_shared<Obstacle>(151, 301, 98, 398));
		map1->addObstacle(make_shared<Obstacle>(751, 301, 98, 398));
		map1->addObstacle(make_shared<Obstacle>(451, 601, 98, 398));
		map1->addObstacle(make_shared<Obstacle>(451, 0, 98, 398));
		maps.push_back(map1);

		std::shared_ptr<Map> map2 = std::make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map2->addUavStart(make_shared<Uav>(80 + i * uavDistance, 50, PI / 2));
		}
		map2->addGoal(make_shared<Goal>(651, 850, 230, 100));
		map2->addGoal(make_shared<Goal>(651, 550, 230, 100));
		map2->addObstacle(make_shared<Obstacle>(600, 650, 350, 200));
		map2->addObstacle(make_shared<Obstacle>(49, 600, 400, 200));
		map2->addObstacle(make_shared<Obstacle>(200, 100, 200, 400));
		map2->addObstacle(make_shared<Obstacle>(100, 400, 100, 100));
		map2->addObstacle(make_shared<Obstacle>(400, 400, 100, 100));
		map2->addObstacle(make_shared<Obstacle>(600, 100, 300, 100));
		map2->addObstacle(make_shared<Obstacle>(700, 200, 200, 299));
		maps.push_back(map2);

		std::shared_ptr<Map> map3 = std::make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map3->addUavStart(make_shared<Uav>(80 + i * uavDistance, 50, PI / 2));
		}
		map3->addGoal(make_shared<Goal>(701, 701, 150, 150));
		map3->addObstacle(make_shared<Obstacle>(300, 300, 200, 400));
		maps.push_back(map3);

		std::shared_ptr<Map> map4 = std::make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map4->addUavStart(make_shared<Uav>(410 + i * uavDistance, 50, PI / 2));
		}
		map4->addGoal(make_shared<Goal>(301, 500, 398, 400));
		map4->addObstacle(make_shared<Obstacle>(300, 300, 400, 200));
		maps.push_back(map4);

		std::shared_ptr<Map> map5 = std::make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map5->addUavStart(make_shared<Uav>(430 + i * uavDistance, 50, PI / 2));
		}
		map5->addGoal(make_shared<Goal>(385, 801, 230, 100));
//		map5->addGoal(make_shared<Goal>(301, 521, 150, 180));
		map5->addGoal(make_shared<Goal>(551, 521, 150, 180));
		map5->addObstacle(make_shared<Obstacle>(199, 699, 601, 101));
		map5->addObstacle(make_shared<Obstacle>(199, 519, 101, 201));
		map5->addObstacle(make_shared<Obstacle>(699, 519, 101, 201));
		map5->addObstacle(make_shared<Obstacle>(199, 199, 601, 101));
		maps.push_back(map5);

		std::shared_ptr<Map> map6 = std::make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map6->addUavStart(make_shared<Uav>(80 + i * uavDistance, 50, PI / 2));
		}
		map6->addGoal(make_shared<Goal>(701, 701, 150, 150));
		map6->addObstacle(make_shared<Obstacle>(400, 0, 200, 470));
		map6->addObstacle(make_shared<Obstacle>(400, 480, 200, 520));
		maps.push_back(map6);

		return maps;
	}

}