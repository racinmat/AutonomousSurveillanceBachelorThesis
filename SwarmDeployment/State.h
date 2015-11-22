#pragma once
#include "PointParticle.h"
#include <vector>
#include <string>
#include "Uav.h"

using namespace std;

namespace App
{

	class State
	{
	public:
		State(int inputCount);
		State(const State& other);
		virtual ~State();
		int index;	//todo: zjistit, k èemu to má být, používá se to v rrt_path. je to jako id, používá se pro získání všech inicializovaných nodes, což já nepotøebuju
		vector<shared_ptr<Uav>> uavs; //spojení promìnných loc a rot z Node objektu z matlabu. nejspíš node bude jiná pro rrt path a pro diskretizaci na nalezen guiding path
		vector<bool> used_inputs; // na zaèátku pole false, o délce number_of_inputs
		shared_ptr<State> prev;
		vector<shared_ptr<Point>> prev_inputs;	//vstupy, které vedly do této node
		bool areAllInputsUsed();
		friend std::ostream& operator<<(std::ostream& os, const State& obj);
	};

}
