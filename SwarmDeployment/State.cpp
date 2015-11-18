#include "State.h"


namespace App
{

	State::State(int inputCount)
	{
		//todo: refactorovat a vyrábìt nové stavy z factory, která jim bude pøedávat délku used_inputs pole podle konfigurace v konstruktoru.
		used_inputs = vector<bool>(inputCount);		//zatím je zde velikost natvrdo
		fill(used_inputs.begin(), used_inputs.end(), false);
	}

	State::State(const State& other) : index(index), used_inputs(used_inputs)
	{
		for (auto uav : other.uavs)
		{
			uavs.push_back(make_shared<Uav>(*uav.get()));
		}
		if (prev)	//kontrola, zda je shred_pointer prázdný
		{
			prev = make_shared<State>(*other.prev.get());
		}
		for (auto prev_input : other.prev_inputs)
		{
			prev_inputs.push_back(make_shared<Point>(*prev_input.get()));
		}
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
		auto newObject = make_shared<State>(used_inputs.size());
		for (auto prev_input : prev_inputs)
		{
			newObject->prev_inputs = prev_inputs;
		}
		newObject->uavs = uavs;
		newObject->index = index;
		newObject->prev = prev;
		newObject->used_inputs = used_inputs;
		return newObject;
	}


	std::ostream& operator<<(std::ostream& os, const State& obj)
	{
		os << "index: " << obj.index << endl;
		os << " uavs: ";
		for (auto a : obj.uavs)
		{
			os << *a.get() << endl;
		}
		os << " used_inputs: ";
		for (auto a : obj.used_inputs)
		{
			os << a;
		}
		if (obj.prev)
		{
			os << " prev: " << *obj.prev.get() << endl;
		} else
		{
			os << " prev: empty" << endl;
		}
		os << " prev_inputs: ";
		for (auto a : obj.prev_inputs)
		{
			os << *a.get() << endl;
		}
		return os;
	}

}
