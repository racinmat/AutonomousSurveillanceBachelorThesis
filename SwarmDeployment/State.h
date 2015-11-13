#pragma once
#include "PointParticle.h"
#include <vector>

using namespace std;

namespace App
{

	class State
	{
	public:
		State();
		virtual ~State();

		int index;	//todo: zjistit, k èemu to má být, používá se to v rrt_path. je to jako id, používá se pro získání všech inicializovaných nodes, což já nepotøebuju

		vector<shared_ptr<App::PointParticle>> uavs; //spojení promìnných loc a rot z Node objektu z matlabu. nejspíš node bude jiná pro rrt path a pro diskretizaci na nalezen guiding path
		vector<bool> used_inputs; // na zaèátku pole false, o délce number_of_inputs
		bool areAllInputsUsed();
		shared_ptr<State> prev;
		vector<double> prev_inputs;	//zatím vùbec nevím, k èemu se tohle používá prostì to jen potøebuju pøedat
	};

}

