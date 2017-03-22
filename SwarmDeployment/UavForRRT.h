#pragma once
#include "PointParticle.h"
#include <vector>
#include "Goal.h"
#include "GuidingPathsCurrentPositions.h"
#include "CarLikeControl.h"
#include "UavInterface.h"

namespace App
{

	class UavForRRT : public UavInterface
	{
	public:
		UavForRRT(const UavForRRT& other);
		explicit UavForRRT(shared_ptr<PointParticle> pointParticle);
		explicit UavForRRT(shared_ptr<Point> location, shared_ptr<Point> rotation);
		explicit UavForRRT(double locationX, double locationY, double rotationZ);
		explicit UavForRRT(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ);
		virtual ~UavForRRT();
		friend ostream& operator<<(ostream& os, const UavForRRT& obj);
		virtual bool isGoalReached() const;
		virtual shared_ptr<GoalInterface> getReachedGoal() const;
		virtual void setReachedGoal(shared_ptr<GoalInterface> reachedGoal);
		virtual shared_ptr<Goal> getConcreteGoal();
		virtual shared_ptr<GuidingPathsCurrentPositions> getCurrentGuidingPathPositions() const;
		Value toJson(Document& d) const;
		static shared_ptr<UavForRRT> fromJson(Value data);
		virtual CarLikeControl getPreviousInput() const;
		virtual void setPreviousInput(const CarLikeControl car_like_control);
		virtual void setPreviousInputStep(double step);

	protected:
		shared_ptr<GoalInterface> reachedGoal;
		shared_ptr<GuidingPathsCurrentPositions> currentGuidingPathPositions;
		CarLikeControl previousInput;
	};

}
