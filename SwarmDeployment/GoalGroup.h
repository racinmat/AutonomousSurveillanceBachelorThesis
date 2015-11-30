#pragma once
#include <memory>
#include "Goal.h"
#include <vector>

using namespace std;

namespace App
{
		
	class GoalGroup : public Goal	// goalGroup dìdí z cíle, abych nemusel nikde v UAV mìnit logiku pøi použití GoalGroup, pokud se v UAV konroluje, zda dorazila do cíle 
	{//todo: zamyslet se, zda by Goal a GoalGroup nemìly implemenovat stejný interface
	public:
		GoalGroup();
		virtual ~GoalGroup();
		virtual void addGoal(shared_ptr<Goal> goal);
		virtual shared_ptr<Point> getMiddle();
		virtual bool contains(shared_ptr<Point> location) override;
		virtual shared_ptr<Rectangle> getRectangle() override;
		virtual shared_ptr<Point> getRandomPointInside() override;
	protected:	//rectangle property of parent will be containing all goals
		vector<shared_ptr<Goal>> goals;
		void initializeRectangle();	//protože jdou cíle pøidávat dynamicky, vytvoøím rectangle lazy, kdy pøedpokládám, že se nic nebude vytváøet. Vyøešilo by se to builder paternem, ale naah.
	};
	
}
