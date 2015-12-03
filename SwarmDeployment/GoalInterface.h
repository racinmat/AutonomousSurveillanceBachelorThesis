#pragma once
#include <memory>
#include "Rectangle.h"

using namespace std;

namespace App
{
	class Goal;

	class GoalInterface
	{
	public:
		virtual ~GoalInterface();
		virtual bool contains(shared_ptr<Point> location) = 0;
		virtual shared_ptr<Rectangle> getRectangle() = 0;
		virtual shared_ptr<Point> getRandomPointInside() = 0;
		virtual shared_ptr<Goal> getConcreteGoal(shared_ptr<Point> location) = 0;
		virtual shared_ptr<Point> getMiddle() = 0;
	};


}
