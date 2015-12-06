#include "PathOptimizer.h"
#include "Random.h"
#include "Dubins/geom/geom.h"
#include "Configuration.h"

using namespace geom;

namespace App
{


	PathOptimizer::PathOptimizer(shared_ptr<DistanceResolver> distanceResolver, shared_ptr<Configuration> configuration, shared_ptr<CarLikeMotionModel> motionModel) :
		distanceResolver(distanceResolver), configuration(configuration), motionModel(motionModel)
	{
	}


	PathOptimizer::~PathOptimizer()
	{
	}

	//optimalizuje cestu pomocí Dubinsových manévrù
	vector<shared_ptr<State>> PathOptimizer::optimizePath(vector<shared_ptr<State>> path)
	{
		int stopLimit = 100;	//kolikrát za sebou se nesmí aplikování Dubinse zlepšit trajektorie, aby se algoritmus zastavil
		int notImprovedCount = 0;

		while (notImprovedCount < stopLimit)
		{
			auto randomFirst = Random::element(path);
			auto randomSecond = Random::element(path);
			while (*randomFirst.get() == *randomSecond.get())
			{
				randomSecond = Random::element(path);
			}
			if (randomFirst->getIndex() > randomSecond->getIndex())
			{
				//swap
				auto temp = randomSecond;
				randomSecond = randomFirst;
				randomFirst = temp;
			}

			for (auto uav : randomSecond->getUavs())
			{

				double distance = 0;
				for (auto i = randomSecond; *i.get() != *randomFirst.get(); i = i->getPrevious())	//i jede od konce po prvek, jehož pøedchùdce je zaèátek
				{
					auto previous = i->getPrevious();
					distance += distanceResolver->getDistance(previous, i);
				}

				//sem pøijde dubins
				auto dubins = new Dubins(uav->getPointParticle()->toPosition(), randomFirst->getUav(uav)->getPointParticle()->toPosition(),	motionModel->getMinimalCurveRadius());
//				switch (dubins->getTypeOfManeuver()) {
//				case TypeOfManeuver::RSR:
//					t1 = dubins->getRadius()*fabs(dubins->getLen1()) / v_max;
//					t2 = dubins->getLen2() / v_max;
//					t3 = dubins->getRadius()*fabs(dubins->getLen3()) / v_max;
//					c1 = c_min;
//					c2 = 0;
//					c3 = c_min;
//					break;
//				case TypeOfManeuver::LSL:
//					t1 = dubins->getRadius()*fabs(dubins->getLen1()) / v_max;
//					t2 = dubins->getLen2() / v_max;
//					t3 = dubins->getRadius()*fabs(dubins->getLen3()) / v_max;
//					c1 = c_max;
//					c2 = 0;
//					c3 = c_max;
//					break;
//				case TypeOfManeuver::LSR:
//					t1 = dubins->getRadius()*fabs(dubins->getLen1()) / v_max;
//					t2 = dubins->getLen2() / v_max;
//					t3 = dubins->getRadius()*fabs(dubins->getLen3()) / v_max;
//					c1 = c_max;
//					c2 = 0;
//					c3 = c_min;
//					break;
//				case TypeOfManeuver::RSL:
//					t1 = dubins->getRadius()*fabs(dubins->getLen1()) / v_max;
//					t2 = dubins->getLen2() / v_max;
//					t3 = dubins->getRadius()*fabs(dubins->getLen3()) / v_max;
//					c1 = c_min;
//					c2 = 0;
//					c3 = c_max;
//					break;
//				case TypeOfManeuver::RLR:
//					t1 = dubins->getRadius()*fabs(dubins->getLen1()) / v_max;
//					t2 = dubins->getRadius()*fabs(dubins->getLen2()) / v_max;
//					t3 = dubins->getRadius()*fabs(dubins->getLen3()) / v_max;
//					c1 = c_min;
//					c2 = c_max;
//					c3 = c_min;
//					break;
//				case TypeOfManeuver::LRL:
//					t1 = dubins->getRadius()*fabs(dubins->getLen1()) / v_max;
//					t2 = dubins->getRadius()*fabs(dubins->getLen2()) / v_max;
//					t3 = dubins->getRadius()*fabs(dubins->getLen3()) / v_max;
//					c1 = c_max;
//					c2 = c_min;
//					c3 = c_max;
//					break;
//				default:
//					break;
//				}

				double newDistance = 0;

				if (newDistance < distance)
				{
					//todo: udìlat validace
					bool isNewPathValid = true;

					if (isNewPathValid)
					{

					}
					//zde se vyhodí delší èást cesty a místo ní se sem dá kratší èást cesty
				}
			}
		}
		return path;
	}


}
