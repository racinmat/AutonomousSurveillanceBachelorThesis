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
		shared_ptr<GuidingPathsCurrentPositions> current_indexes;
		friend bool operator<(const Uav& lhs, const Uav& rhs);	//kvùli používání jako klíèe v std::map
		friend bool operator<=(const Uav& lhs, const Uav& rhs);
		friend bool operator>(const Uav& lhs, const Uav& rhs);
		friend bool operator>=(const Uav& lhs, const Uav& rhs);
		friend bool operator==(const Uav& lhs, const Uav& rhs);
		friend bool operator!=(const Uav& lhs, const Uav& rhs);
		size_t hash_value() const;
		virtual int getId() const;

	protected:
		shared_ptr<PointParticle> pointParticle;
		shared_ptr<GoalInterface> reachedGoal;
		int id;
		static int lastId;
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
