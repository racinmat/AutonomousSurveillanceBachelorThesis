#pragma once
#include <memory>
#include "Rectangle.h"

using namespace std;

namespace App
{

	class GoalInterface
	{
	public:
		virtual ~GoalInterface();
		virtual bool contains(shared_ptr<Point> location) = 0;
		virtual shared_ptr<Rectangle> getRectangle() = 0;
		virtual shared_ptr<Point> getRandomPointInside() = 0;
		//todo: udìlat nìjaké getGoal, aby se v GoalGroup zjistilo, v jakém cíli UAV je. nebo tak nìco. a možná to ani nebude tøeba.
	};


}
