#include "State.h"


namespace App
{
	int LinkedState::lastIndex = 0;

	LinkedState::LinkedState(int inputCount) : index(lastIndex++), time(0)
	{
		//todo: refactorovat a vyrábìt nové stavy z factory, která jim bude pøedávat délku used_inputs pole podle konfigurace v konstruktoru.
		used_inputs = vector<bool>(inputCount);		//zatím je zde velikost natvrdo
		fill(used_inputs.begin(), used_inputs.end(), false);
	}

	LinkedState::LinkedState(const LinkedState& other) : index(other.index + 1), used_inputs(other.used_inputs), time(other.time)
	{
		for (auto uav : other.uavs)
		{
			uavs.push_back(make_shared<Uav>(*uav.get()));
		}
		if (other.previous)	//kontrola, zda je shred_pointer prázdný
		{
			previous = other.previous;
		}
	}

	LinkedState::~LinkedState()
	{
	}

	bool LinkedState::areAllInputsUsed()
	{
		bool areAllInputsUsed = true;
		for (auto inputUsed : used_inputs)
		{
			areAllInputsUsed = areAllInputsUsed && inputUsed;
		}
		return areAllInputsUsed;
	}

	shared_ptr<Uav> LinkedState::getUav(shared_ptr<Uav> uav) const
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

	bool LinkedState::areUavsInGoals()
	{
		bool allInGoals = true;
		for (auto uav : uavs)
		{
			allInGoals = allInGoals && uav->isGoalReached();
		}
		return allInGoals;
	}

	double LinkedState::getDistanceOfNewNodes() const
	{
		return distanceOfNewNodes;
	}

	void LinkedState::setDistanceOfNewNodes(const double distance_of_new_nodes)
	{
		distanceOfNewNodes = distance_of_new_nodes;
	}

	int LinkedState::getIndex() const
	{
		return index;
	}

	shared_ptr<LinkedState> LinkedState::getPrevious() const
	{
		return previous;
	}

	void LinkedState::setPrevious(const shared_ptr<LinkedState> state)
	{
		previous = state;
	}

	vector<shared_ptr<Uav>> LinkedState::getUavs() const
	{
		return uavs;
	}

	void LinkedState::setUavs(const vector<shared_ptr<Uav>> shared_ptrs)
	{
		uavs = shared_ptrs;
	}

	void LinkedState::incrementTime(double increment)
	{
		time += increment;
	}

	double LinkedState::getTime() const
	{
		return time;
	}

	std::ostream& operator<<(std::ostream& os, const LinkedState& obj)
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
		return os;
	}

	bool operator==(const LinkedState& lhs, const LinkedState& rhs)
	{
		for (auto uav : lhs.uavs)
		{
			if (*uav->getPointParticle().get() != *rhs.getUav(uav)->getPointParticle().get())
			{
				return false;
			}
		}
		return true;
	}

	bool operator!=(const LinkedState& lhs, const LinkedState& rhs)
	{
		return !(lhs == rhs);
	}
}
