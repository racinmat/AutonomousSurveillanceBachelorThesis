#include "EmptyGoal.h"

namespace App
{


	EmptyGoal::EmptyGoal()
	{
	}


	EmptyGoal::~EmptyGoal()
	{
	}

	void EmptyGoal::addGoal(shared_ptr<Goal> goal)
	{
		goals.push_back(goal);
	}

	shared_ptr<Point> EmptyGoal::getMiddle()
	{
		if (!rectangle)
		{
			initializeRectangle();
		}
		return rectangle->getMiddle();
	}

	bool EmptyGoal::contains(shared_ptr<Point> location)
	{
		if (!rectangle)
		{
			initializeRectangle();
		}
		return rectangle->contains(location);
	}

	shared_ptr<Rectangle> EmptyGoal::getRectangle()
	{
		if (!rectangle)
		{
			initializeRectangle();
		}
		return rectangle;
	}

	shared_ptr<Point> EmptyGoal::getRandomPointInside()
	{
		shared_ptr<Goal> randomGoal = Random::element(goals);
		return randomGoal->getRandomPointInside();
	}

	shared_ptr<Goal> EmptyGoal::getConcreteGoal(shared_ptr<Point> location)
	{
		for (auto goal : goals)
		{
			if (goal->contains(location))
			{
				return goal;
			}
		}
		throw "Goal not found";
	}

}
