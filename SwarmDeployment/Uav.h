#pragma once
#include "PointParticle.h"
#include <vector>
#include "Goal.h"
#include "GuidingPathsCurrentPositions.h"

namespace App
{

	class Uav
	{
	public:
		Uav(const Uav& other);
		Uav(shared_ptr<PointParticle> pointParticle);
		Uav(shared_ptr<Point> location, shared_ptr<Point> rotation);
		Uav(double locationX, double locationY, double rotationZ);
		Uav(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ);
		virtual ~Uav();
		virtual shared_ptr<PointParticle> getPointParticle() const;
		friend ostream& operator<<(ostream& os, const Uav& obj);
		virtual bool isGoalReached() const;
		virtual shared_ptr<Goal> getReachedGoal() const;
		virtual void setReachedGoal(shared_ptr<Goal> reachedGoal);
		shared_ptr<GuidingPathsCurrentPositions> current_indexes;
		friend bool operator<(const Uav& lhs, const Uav& rhs);	//kvùli používání jako klíèe v std::map
		friend bool operator<=(const Uav& lhs, const Uav& rhs);
		friend bool operator>(const Uav& lhs, const Uav& rhs);
		friend bool operator>=(const Uav& lhs, const Uav& rhs);

	protected:
		shared_ptr<PointParticle> pointParticle;
		shared_ptr<Goal> reachedGoal;
		int id;
		static int lastId;
	};

}
