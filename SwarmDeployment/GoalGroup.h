#pragma once
#include <memory>
#include "Goal.h"

using namespace App;

namespace App
{
		
	class GoalGroup
	{
	public:
		GoalGroup();
		virtual ~GoalGroup();
		void addGoal(shared_ptr<Goal> goal);
		shared_ptr<Point> getMiddle();
		bool contains(shared_ptr<Point> location);

	protected:
		vector<shared_ptr<Goal>> goals;
		shared_ptr<Rectangle> rectangle;	//rectangle containing all goals
		void initializeRectangle();	//protože jdou cíle pøidávat dynamicky, vytvoøím rectangle lazy, kdy pøedpokládám, že se nic nebude vytváøet. Vyøešilo by se to builder paternem, ale naah.
	};
	
}
