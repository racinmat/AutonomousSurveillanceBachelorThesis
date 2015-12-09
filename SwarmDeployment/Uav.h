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
		explicit Uav(shared_ptr<PointParticle> pointParticle);
		explicit Uav(shared_ptr<Point> location, shared_ptr<Point> rotation);
		explicit Uav(double locationX, double locationY, double rotationZ);
		explicit Uav(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ);
		virtual ~Uav();
		virtual shared_ptr<PointParticle> getPointParticle() const;
		friend ostream& operator<<(ostream& os, const Uav& obj);
		virtual bool isGoalReached() const;
		virtual shared_ptr<GoalInterface> getReachedGoal() const;
		virtual void setReachedGoal(shared_ptr<GoalInterface> reachedGoal);
		friend bool operator<(const Uav& lhs, const Uav& rhs);	//kvùli používání jako klíèe v std::map
		friend bool operator<=(const Uav& lhs, const Uav& rhs);
		friend bool operator>(const Uav& lhs, const Uav& rhs);
		friend bool operator>=(const Uav& lhs, const Uav& rhs);
		friend bool operator==(const Uav& lhs, const Uav& rhs);
		friend bool operator!=(const Uav& lhs, const Uav& rhs);
		size_t hash_value() const;
		virtual int getId() const;
		virtual shared_ptr<Goal> getConcreteGoal();
		virtual shared_ptr<GuidingPathsCurrentPositions> getCurrentGuidingPathPositions() const;
		virtual void setId(const int id);
		virtual void setPointParticle(const shared_ptr<PointParticle> point_particle);

	protected:
		shared_ptr<PointParticle> pointParticle;
		shared_ptr<GoalInterface> reachedGoal;
		int id;
		static int lastId;
		shared_ptr<GuidingPathsCurrentPositions> currentGuidingPathPositions;
	};


	class UavHasher
	{
	public:
		size_t operator() (Uav const& key) const
		{
			return key.hash_value();
		}
	};

}
