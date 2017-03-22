#include "State.h"

namespace App
{
	State::State()
	{
	}

	State::~State()
	{
	}

	State::State(shared_ptr<LinkedState> other)
	{
		for (auto uav : other->getUavsForRRT())
		{
			auto newUav = make_shared<Uav>(*uav.get());
			uavs.push_back(newUav);
		}
	}

	State::State(const State& other)
	{
		for (auto uav : other.getUavs())
		{
			uavs.push_back(make_shared<Uav>(*uav.get()));
		}
	}

	shared_ptr<UavInterface> State::getBaseUav(shared_ptr<UavInterface> uav) const
	{
		for (auto other : uavs)
		{
			if (*other.get() == *uav.get())
			{
				return other;
			}
		}
		throw "No equal node found for node " + to_string(uav->getId());
	}

	shared_ptr<Uav> State::getUav(shared_ptr<Uav> uav) const
	{
		for (auto other : uavs)
		{
			if (*other.get() == *uav.get())
			{
				return other;
			}
		}
		throw "No equal node found for node " + to_string(uav->getId());
	}

	vector<shared_ptr<UavInterface>> State::getBaseUavs() const
	{
		//podle http://stackoverflow.com/questions/28823540/passing-vectors-of-derived-shared-pointers
//		vector<shared_ptr<UavInterface>> baseUavs;
//		baseUavs.reserve(uavs.size());
//		std::transform(begin(uavs), end(uavs), std::back_inserter(baseUavs),
//			[](const std::shared_ptr<Uav>& shptr)
//		{
//			return std::dynamic_pointer_cast<Uav>(shptr);
//		});
//		return baseUavs;

		vector<shared_ptr<UavInterface>> baseUavs = vector<shared_ptr<UavInterface>>(uavs.size());
		int i = 0;
		for (auto uav : uavs)
		{
			baseUavs[i] = uav;
			i++;
		}
		return baseUavs;
	}

	vector<shared_ptr<Uav>> State::getUavs() const
	{
		return uavs;
	}

	void State::swapUavs(shared_ptr<Uav> first, shared_ptr<Uav> second)
	{
		auto firstUav = getBaseUav(first);
		auto secondUav = getBaseUav(second);
		int tempId = firstUav->getId();
		firstUav->setId(secondUav->getId());
		secondUav->setId(tempId);
	}

	Value State::toJson(Document& d) const
	{
		Value object(kObjectType);
		Document::AllocatorType& allocator = d.GetAllocator();
		for (auto uav : this->uavs)
		{
			object.AddMember(Value{}.SetString(to_string(uav->getId()).c_str(), to_string(uav->getId()).length(), allocator), uav->toJson(d), allocator);
		}
		return object;
	}

	shared_ptr<State> State::fromJson(Value& data)
	{
		auto state = make_shared<State>();
		for (auto &uavData : data.GetArray())
		{
			state->uavs.push_back(Uav::fromJson(uavData));
		}
		return state;
	}

	ostream& operator<<(ostream& os, const State& obj)
	{
		os << " uavs: " << endl;
		for (auto a : obj.uavs)
		{
			os << *a.get() << endl;
		}
		return os;
	}

	bool operator==(const State& lhs, const State& rhs)
	{
		for (auto uav : lhs.uavs)
		{
			if (*uav->getPointParticle().get() != *rhs.getBaseUav(uav)->getPointParticle().get())
			{
				return false;
			}
		}
		return true;
	}

	bool operator!=(const State& lhs, const State& rhs)
	{
		return !(lhs == rhs);
	}
}