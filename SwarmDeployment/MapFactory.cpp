#include "MapFactory.h"
#include <memory>
#include "Uav.h"

#define PI 3.14159265358979323846

namespace App
{

	vector<shared_ptr<Map>> MapFactory::createMaps(int uavCount)
	{
		int uavDistance = 30;

		auto maps = vector<shared_ptr<Map>>();

		shared_ptr<Map> map0 = make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map0->addUavStart(make_shared<Uav>(80 + i * uavDistance, 50, PI/2));
		}
		map0->addGoal(make_shared<Goal>(651, 701, 348, 148));
		map0->addObstacle(make_shared<Obstacle>(0, 401, 698, 48));
		map0->addObstacle(make_shared<Obstacle>(251, 601, 748, 48));
		map0->addObstacle(make_shared<Obstacle>(401, 0, 48, 198));
		map0->addObstacle(make_shared<Obstacle>(201, 601, 48, 198));
		map0->addObstacle(make_shared<Obstacle>(651, 201, 48, 198));
		maps.push_back(map0);

		shared_ptr<Map> map1 = make_shared<Map>();
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

		shared_ptr<Map> map2 = make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map2->addUavStart(make_shared<Uav>(80 + i * uavDistance, 50, PI / 2));
		}
		map2->addGoal(make_shared<Goal>(651, 850, 230, 100));
		map2->addGoal(make_shared<Goal>(651, 550, 230, 100));
		map2->addObstacle(make_shared<Obstacle>(601, 651, 348, 198));
		map2->addObstacle(make_shared<Obstacle>(51, 601, 398, 198));
		map2->addObstacle(make_shared<Obstacle>(201, 101, 198, 398));
		map2->addObstacle(make_shared<Obstacle>(101, 401, 98, 98));
		map2->addObstacle(make_shared<Obstacle>(401, 401, 98, 98));
		map2->addObstacle(make_shared<Obstacle>(601, 101, 298, 98));
		map2->addObstacle(make_shared<Obstacle>(701, 201, 198, 298));
		maps.push_back(map2);

		shared_ptr<Map> map3 = make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map3->addUavStart(make_shared<Uav>(80 + i * uavDistance, 50, PI / 2));
		}
		map3->addGoal(make_shared<Goal>(701, 701, 150, 150));
		map3->addObstacle(make_shared<Obstacle>(301, 301, 198, 398));
		maps.push_back(map3);

		shared_ptr<Map> map4 = make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map4->addUavStart(make_shared<Uav>(410 + i * uavDistance, 50, PI / 2));
		}
		map4->addGoal(make_shared<Goal>(301, 500, 398, 400));
		map4->addObstacle(make_shared<Obstacle>(300, 300, 400, 200));
		maps.push_back(map4);

		shared_ptr<Map> map5 = make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map5->addUavStart(make_shared<Uav>(430 + i * uavDistance, 50, PI / 2));
		}
		map5->addGoal(make_shared<Goal>(385, 801, 230, 100));
		map5->addGoal(make_shared<Goal>(301, 521, 150, 180));
		map5->addGoal(make_shared<Goal>(551, 521, 150, 180));
		map5->addObstacle(make_shared<Obstacle>(201, 701, 598, 98));
		map5->addObstacle(make_shared<Obstacle>(201, 519, 98, 198));
		map5->addObstacle(make_shared<Obstacle>(701, 519, 98, 198));
		map5->addObstacle(make_shared<Obstacle>(201, 201, 598, 98));
		maps.push_back(map5);

		shared_ptr<Map> map6 = make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map6->addUavStart(make_shared<Uav>(80 + i * uavDistance, 50, PI / 2));
		}
		map6->addGoal(make_shared<Goal>(701, 701, 150, 150));
		map6->addObstacle(make_shared<Obstacle>(400, 0, 200, 470));
		map6->addObstacle(make_shared<Obstacle>(400, 480, 200, 520));
//		map6->addObstacle(make_shared<Obstacle>(400, 0, 200, 49));
//		map6->addObstacle(make_shared<Obstacle>(400, 76, 200, 919));
		maps.push_back(map6);

		shared_ptr<Map> map7 = make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map7->addUavStart(make_shared<Uav>(80 + i * uavDistance, 50, PI / 2));
		}
		map7->addGoal(make_shared<Goal>(100, 200, 150, 150));
		maps.push_back(map7);

		return maps;
	}

}