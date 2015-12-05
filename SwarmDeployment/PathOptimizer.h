#pragma once
#include "State.h"

namespace App
{

	class PathOptimizer
	{
	public:
		PathOptimizer();
		virtual ~PathOptimizer();
		vector<shared_ptr<State>> optimizePath(vector<shared_ptr<State>> path);
	};


}
