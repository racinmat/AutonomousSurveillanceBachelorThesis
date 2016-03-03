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
		explicit Uav(const UavForRRT& other);
		Uav(const Uav& other);
		explicit Uav(shared_ptr<PointParticle> pointParticle);
		explicit Uav(shared_ptr<Point> location, shared_ptr<Point> rotation);
		explicit Uav(double locationX, double locationY, double rotationZ);
		explicit Uav(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ);
		virtual ~Uav();
		friend ostream& operator<<(ostream& os, const Uav& obj);
		virtual bool isGoalReached() const;
		virtual shared_ptr<GoalInterface> getReachedGoal() const;
		virtual void setReachedGoal(shared_ptr<GoalInterface> reachedGoal);
		virtual shared_ptr<Goal> getConcreteGoal();
		virtual shared_ptr<GuidingPathsCurrentPositions> getCurrentGuidingPathPositions() const;
		mObject toJson() const;
		static shared_ptr<Uav> fromJson(mValue data);
		virtual CarLikeControl getPreviousInput() const;
		virtual void setPreviousInput(const CarLikeControl car_like_control);
		virtual void setPreviousInputStep(double step);

	protected:
		shared_ptr<GoalInterface> reachedGoal;
		shared_ptr<GuidingPathsCurrentPositions> currentGuidingPathPositions;
		CarLikeControl previousInput;
	};


}
