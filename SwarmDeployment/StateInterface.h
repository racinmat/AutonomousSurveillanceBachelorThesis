#pragma once
#include "UavForRRT.h"

using namespace std;

namespace App
{

	class StateInterface
	{
	public:
		virtual ~StateInterface();
		virtual shared_ptr<UavInterface> getBaseUav(shared_ptr<UavInterface> uav) const = 0;	//used to acquire uav with same id as uav in argument, even if uav locations differ. It uses == to compare
		virtual vector<shared_ptr<UavInterface>> getBaseUavs() const = 0;
	};

}