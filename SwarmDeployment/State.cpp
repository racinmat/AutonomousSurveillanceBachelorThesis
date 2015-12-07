#include "State.h"


namespace App
{
	int State::lastIndex = 0;

	State::State(int inputCount) : index(lastIndex++), time(0)
	{
		//todo: refactorovat a vyrábìt nové stavy z factory, která jim bude pøedávat délku used_inputs pole podle konfigurace v konstruktoru.
		used_inputs = vector<bool>(inputCount);		//zatím je zde velikost natvrdo
		fill(used_inputs.begin(), used_inputs.end(), false);
	}

	State::State(const State& other) : index(other.index + 1), used_inputs(other.used_inputs), time(other.time)
	{
		for (auto uav : other.uavs)
		{
			uavs.push_back(make_shared<Uav>(*uav.get()));
		}
		if (previous)	//kontrola, zda je shred_pointer prázdný
		{
			previous = make_shared<State>(*other.previous.get());
		}
		for (auto prev_input : other.prev_inputs)
		{
			prev_inputs[prev_input.first] = make_shared<CarLikeControl>(*prev_input.second.get());
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

	shared_ptr<Uav> State::getUav(shared_ptr<Uav> uav)
	{
		for(auto other : uavs)
		{
			if (*other.get() == *uav.get())
			{
				return other;
			}
		}
		throw "No equal node found for node " + to_string(uav->getId());
	}

	bool State::areUavsInGoals()
	{
		bool allInGoals = true;
		for (auto uav : uavs)
		{
			allInGoals = allInGoals && uav->isGoalReached();
		}
		return allInGoals;
	}

	double State::getDistanceOfNewNodes() const
	{
		return distanceOfNewNodes;
	}

	void State::setDistanceOfNewNodes(const double distance_of_new_nodes)
	{
		distanceOfNewNodes = distance_of_new_nodes;
	}

	int State::getIndex() const
	{
		return index;
	}

	shared_ptr<State> State::getPrevious() const
	{
		return previous;
	}

	void State::setPrevious(const shared_ptr<State> state)
	{
		previous = state;
	}

	vector<shared_ptr<Uav>> State::getUavs() const
	{
		return uavs;
	}

	void State::setUavs(const vector<shared_ptr<Uav>> shared_ptrs)
	{
		uavs = shared_ptrs;
	}

	void State::incrementTime(double increment)
	{
		time += increment;
	}

	double State::getTime() const
	{
		return time;
	}

	void State::swapUavs(shared_ptr<Uav> first, shared_ptr<Uav> second)
	{
		getUav(first)->
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
		if (obj.previous)
		{
			os << " prev: " << *obj.previous.get() << endl;
		} else
		{
			os << " prev: empty" << endl;
		}
		os << " prev_inputs: ";
		for (auto a : obj.prev_inputs)
		{
			os << "id " << a.first << ": " << *a.second.get() << endl;
		}
		return os;
	}

	bool operator==(const State& lhs, const State& rhs)
	{
		return lhs.uavs == rhs.uavs
			&& lhs.used_inputs == rhs.used_inputs
			&& lhs.prev_inputs == rhs.prev_inputs
			&& lhs.distanceOfNewNodes == rhs.distanceOfNewNodes
			&& lhs.previous == rhs.previous
			&& lhs.index == rhs.index;
	}

	bool operator!=(const State& lhs, const State& rhs)
	{
		return !(lhs == rhs);
	}
}
