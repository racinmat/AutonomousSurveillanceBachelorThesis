#pragma once
#include "PointParticle.h"
#include <vector>
#include <string>
#include "Uav.h"
#include "CarLikeControl.h"

using namespace std;

namespace App
{

	class State
	{
	public:
		State(int inputCount);
		State(const State& other);
		virtual ~State();
		vector<bool> used_inputs; // na zaèátku pole false, o délce number_of_inputs
		unordered_map<Uav, shared_ptr<CarLikeControl>, UavHasher> prev_inputs;	//vstupy, které vedly do této node
		bool areAllInputsUsed();
		friend std::ostream& operator<<(std::ostream& os, const State& obj);
		shared_ptr<Uav> getUav(shared_ptr<Uav> uav);	//used to acquire uav with same id as uav in argument, even if uav locations differ. It uses == to compare
		bool areUavsInGoals();
		virtual double getDistanceOfNewNodes() const;
		virtual void setDistanceOfNewNodes(const double distance_of_new_nodes);
		virtual int getIndex() const;
		virtual shared_ptr<State> getPrevious() const;
		virtual void setPrevious(const shared_ptr<State> state);
		friend bool operator==(const State& lhs, const State& rhs);
		friend bool operator!=(const State& lhs, const State& rhs);
		virtual vector<shared_ptr<Uav>> getUavs() const;
		virtual void setUavs(const vector<shared_ptr<Uav>> shared_ptrs);
		virtual void incrementTime(double increment);
		virtual double getTime() const;
		virtual void swapUavs(shared_ptr<Uav> first, shared_ptr<Uav> second);

	protected:
		vector<shared_ptr<Uav>> uavs; //spojení promìnných loc a rot z Node objektu z matlabu. nejspíš node bude jiná pro rrt path a pro diskretizaci na nalezen guiding path
		double distanceOfNewNodes;	//protože se mìní, ukládám ji sem
		shared_ptr<State> previous;
		int index;
		static int lastIndex;
		double time;	//èas, ve kterém jsou UAV v daných pozicích. Pokud chci kontrolovat kolize, potøebuji vìdìt, v jakém èase se UAV na daném místì nachází.
	};

}
