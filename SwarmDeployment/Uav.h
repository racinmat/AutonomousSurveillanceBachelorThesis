#pragma once
#include "PointParticle.h"
#include <vector>
#include "Goal.h"
#include "GuidingPathsCurrentPositions.h"
#include <json_spirit_v4.08/json_spirit/json_spirit_reader.h>
#include "CarLikeControl.h"
#include "UavInterface.h"

using namespace json_spirit;

namespace App
{

	class Uav : public UavInterface
	{
	public:
		Uav(const Uav& other);
		explicit Uav(shared_ptr<Point> location, shared_ptr<Point> rotation);
		explicit Uav(double locationX, double locationY, double rotationZ);
		explicit Uav(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ);
		virtual ~Uav();
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
		mObject toJson() const;
		static shared_ptr<Uav> fromJson(mValue data);
		virtual CarLikeControl getPreviousInput() const;
		virtual void setPreviousInput(const CarLikeControl car_like_control);
		virtual void setPreviousInputStep(double step);

	protected:
		shared_ptr<GoalInterface> reachedGoal;
		int id;
		static int lastId;
		shared_ptr<GuidingPathsCurrentPositions> currentGuidingPathPositions;
		CarLikeControl previousInput;
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
