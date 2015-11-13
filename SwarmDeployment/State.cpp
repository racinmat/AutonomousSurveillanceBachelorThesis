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

	shared_ptr<State> State::clone() const
	{
		auto newObject = make_shared<State>();
		newObject->prev_inputs = prev_inputs;
		newObject->uavs = uavs;
		newObject->index = index;
		newObject->prev = prev;
		newObject->used_inputs = used_inputs;
		return newObject;
	}
}