#pragma once
#include "LinkedState.h"
#include "StateInterface.h"
#include "Uav.h"

using namespace std;

namespace App
{

	class State : public StateInterface
	{
	public:
		State();
		virtual ~State();
		explicit State(shared_ptr<LinkedState> other);
		explicit State(const State& other);
		friend ostream& operator<<(ostream& os, const State& obj);
		virtual shared_ptr<UavInterface> getBaseUav(shared_ptr<UavInterface> uav) const override;	//used to acquire uav with same id as uav in argument, even if uav locations differ. It uses == to compare
		virtual shared_ptr<Uav> getUav(shared_ptr<Uav> uav) const;	//used to acquire uav with same id as uav in argument, even if uav locations differ. It uses == to compare
		virtual vector<shared_ptr<Uav>> getUavs() const;
		friend bool operator==(const State& lhs, const State& rhs);
		friend bool operator!=(const State& lhs, const State& rhs);
		virtual vector<shared_ptr<UavInterface>> getBaseUavs() const override;
		virtual void swapUavs(shared_ptr<Uav> first, shared_ptr<Uav> second);
		virtual Value toJson(Document::AllocatorType& allocator) const;
		static shared_ptr<State> fromJson(Value& data);
	protected:
		vector<shared_ptr<Uav>> uavs;
	};

}
