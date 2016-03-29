#pragma once
#include "PointParticle.h"
#include <vector>
#include <string>
#include "UavForRRT.h"
#include "CarLikeControl.h"
#include "StateInterface.h"

using namespace std;

namespace App
{

	class LinkedState : public StateInterface
	{
	public:
		LinkedState(int inputCount);
		LinkedState(const LinkedState& other);
		virtual ~LinkedState();
		vector<bool> used_inputs; // na zaèátku pole false, o délce number_of_inputs
		bool areAllInputsUsed();
		friend std::ostream& operator<<(std::ostream& os, const LinkedState& obj);
		virtual shared_ptr<UavInterface> getBaseUav(shared_ptr<UavInterface> uav) const override;	//used to acquire uav with same id as uav in argument, even if uav locations differ. It uses == to compare
		bool areUavsInGoals() const;
		virtual double getDistanceOfNewNodes() const;
		virtual void setDistanceOfNewNodes(const double distance_of_new_nodes);
		virtual int getIndex() const;
		virtual shared_ptr<LinkedState> getPrevious() const;
		virtual void setPrevious(const shared_ptr<LinkedState> state);
		friend bool operator==(const LinkedState& lhs, const LinkedState& rhs);
		friend bool operator!=(const LinkedState& lhs, const LinkedState& rhs);
		virtual vector<shared_ptr<UavInterface>> getBaseUavs() const override;
		virtual void setUavs(const vector<shared_ptr<UavForRRT>> shared_ptrs);
		virtual void incrementTime(double increment);
		virtual double getTime() const;
		virtual vector<shared_ptr<UavForRRT>> getUavsForRRT();
	protected:
		vector<shared_ptr<UavForRRT>> uavs; //spojení promìnných loc a rot z Node objektu z matlabu. nejspíš node bude jiná pro rrt path a pro diskretizaci na nalezen guiding path
		double distanceOfNewNodes;	//protože se mìní, ukládám ji sem
		shared_ptr<LinkedState> previous;
		int index;
		static int lastIndex;
		double time;	//èas, ve kterém jsou UAV v daných pozicích. Pokud chci kontrolovat kolize, potøebuji vìdìt, v jakém èase se UAV na daném místì nachází.
	};

}
