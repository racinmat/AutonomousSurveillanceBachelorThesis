#pragma once
#include "vector"
#include <memory>
#include "Node.h"
#include "State.h"

using namespace std;

namespace App
{
	class Output
	{
	public:
		Output();
		virtual ~Output();
		vector<shared_ptr<State>> nodes;
		vector<int> distance_of_new_nodes;
		int start_goal_distance_euclidean;
		int start_goal_distance_a_star;
		vector<int> goal_reached;
	};

}

