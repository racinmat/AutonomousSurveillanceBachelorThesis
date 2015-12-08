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
		vector<shared_ptr<LinkedState>> nodes;
		int start_goal_distance_euclidean;	//distance between start and goal. 
		int start_goal_distance_a_star;	//todo: tyto dva integery pøedìlat na pole doublù, pole dlouhé jako poèet cílù
		bool goals_reached;	//empty pointer, pokud uav nedorazilo. vektor dlouhý jako poèet uav
		//todo: pøedìlat na mapy
		vector<double> distancesToGoal;		//ke každé UAV délka cesty k cíli
		vector<shared_ptr<LinkedState>> finalNodes;
	};

}

