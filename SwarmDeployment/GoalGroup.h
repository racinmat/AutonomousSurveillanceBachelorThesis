#pragma once
#include <memory>
#include "Goal.h"
#include <vector>
#include "GoalInterface.h"

using namespace std;

namespace App
{
		
	class GoalGroup : public GoalInterface	// goalGroup d�d� z c�le, abych nemusel nikde v UAV m�nit logiku p�i pou�it� GoalGroup, pokud se v UAV konroluje, zda dorazila do c�le 
	{//todo: zamyslet se, zda by Goal a GoalGroup nem�ly implemenovat stejn� interface
	public:
		GoalGroup();
		virtual ~GoalGroup();
		virtual void addGoal(shared_ptr<Goal> goal);
		virtual shared_ptr<Point> getMiddle() override;
		virtual bool contains(shared_ptr<Point> location) override;
		virtual shared_ptr<Rectangle> getRectangle() override;
		virtual shared_ptr<Point> getRandomPointInside() override;
		shared_ptr<Goal> getConcreteGoal(shared_ptr<Point> location) override;

	protected:	//rectangle property of parent will be containing all goals
		vector<shared_ptr<Goal>> goals;
		shared_ptr<Rectangle> rectangle;
		void initializeRectangle();	//proto�e jdou c�le p�id�vat dynamicky, vytvo��m rectangle lazy, kdy p�edpokl�d�m, �e se nic nebude vytv��et. Vy�e�ilo by se to builder paternem, ale naah.
	};
	
}
