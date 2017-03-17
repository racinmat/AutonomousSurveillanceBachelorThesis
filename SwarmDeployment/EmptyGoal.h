#pragma once
#include "GoalInterface.h"

namespace App
{
	
	class EmptyGoal : public GoalInterface
	{
	
	public:
		EmptyGoal();
		virtual ~EmptyGoal();
		virtual bool contains(shared_ptr<Point> location) override;
		virtual shared_ptr<Rectangle> getRectangle() override;
		virtual shared_ptr<Point> getRandomPointInside() override;
		virtual shared_ptr<Goal> getConcreteGoal(shared_ptr<Point> location) override;
		virtual shared_ptr<Point> getMiddle() override;	
	};

}
