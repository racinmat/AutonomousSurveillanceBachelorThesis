#include "MapFactory.h"
#include <memory>
#include "UavForRRT.h"

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
			map0->addUavStart(make_shared<UavForRRT>(80 + i * uavDistance, 50, PI/2));
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
			map1->addUavStart(make_shared<UavForRRT>(431 + i * uavDistance, 501, PI / 2));
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
			map2->addUavStart(make_shared<UavForRRT>(80 + i * uavDistance, 50, PI / 2));
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
			map3->addUavStart(make_shared<UavForRRT>(80 + i * uavDistance, 50, PI / 2));
		}
		map3->addGoal(make_shared<Goal>(701, 701, 150, 150));
		map3->addObstacle(make_shared<Obstacle>(301, 301, 198, 398));
		maps.push_back(map3);

		shared_ptr<Map> map4 = make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map4->addUavStart(make_shared<UavForRRT>(410 + i * uavDistance, 50, PI / 2));
		}
		map4->addGoal(make_shared<Goal>(301, 500, 398, 400));
		map4->addObstacle(make_shared<Obstacle>(300, 300, 400, 200));
		maps.push_back(map4);

		shared_ptr<Map> map5 = make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map5->addUavStart(make_shared<UavForRRT>(430 + i * uavDistance, 50, PI / 2));
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
			map6->addUavStart(make_shared<UavForRRT>(80 + i * uavDistance, 50, PI / 2));
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
			map7->addUavStart(make_shared<UavForRRT>(80 + i * uavDistance, 50, PI / 2));
		}
		map7->addGoal(make_shared<Goal>(200, 50, 150, 150));
		maps.push_back(map7);

		shared_ptr<Map> map8 = make_shared<Map>();				//100 pixelů je 1 metr
		map8->addUavStart(make_shared<UavForRRT>(450, 50, PI / 2));
		map8->addUavStart(make_shared<UavForRRT>(600, 200, PI / 2));
		map8->addUavStart(make_shared<UavForRRT>(750, 50, PI / 2));

		map8->addGoal(make_shared<Goal>(1, 450, 400, 350));
		map8->addObstacle(make_shared<Obstacle>(200, 200, 100, 100));
		map8->addObstacle(make_shared<Obstacle>(300, 300, 100, 100));
		maps.push_back(map8);

		shared_ptr<Map> map9 = make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map9->addUavStart(make_shared<UavForRRT>(400 + i * uavDistance, 50, PI / 2));
		}

		map9->addGoal(make_shared<Goal>(300, 600, 150, 200));
		map9->addGoal(make_shared<Goal>(500, 600, 150, 200));
		map9->addObstacle(make_shared<Obstacle>(450, 600, 50, 400));
		map9->addObstacle(make_shared<Obstacle>(300, 550, 350, 50));
		maps.push_back(map9);

		shared_ptr<Map> map10 = make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
//			map10->addUavStart(make_shared<UavForRRT>(400 + i * uavDistance, 50, PI / 2));
			map10->addUavStart(make_shared<UavForRRT>(50 + i * uavDistance, 50, PI / 2));
		}

		map10->addGoal(make_shared<Goal>(1, 600, 400, 400));
		map10->addGoal(make_shared<Goal>(551, 600, 450, 400));
		map10->addObstacle(make_shared<Obstacle>(400, 550, 150, 450));
		maps.push_back(map10);

		shared_ptr<Map> map11 = make_shared<Map>();
		for (int i = 0; i < uavCount; i++)
		{
			map11->addUavStart(make_shared<UavForRRT>(280 + i * uavDistance, 50, PI / 2));
		}
		map11->addGoal(make_shared<Goal>(001, 501, 200, 200));
		map11->addGoal(make_shared<Goal>(801, 501, 200, 200));
		maps.push_back(map11);

		return maps;
	}

}
