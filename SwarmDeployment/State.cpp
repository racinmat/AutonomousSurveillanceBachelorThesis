#include "State.h"


namespace App
{

	State::State()
	{
	}


	State::~State()
	{
	}

	bool State::areAllInputsUsed()
	{
		bool areAllInputsUsed = true;
		for (auto inputUsed : used_inputs)
		{
			areAllInputsUsed = areAllInputsUsed && inputUsed;
		}
		return areAllInputsUsed;
	}
}