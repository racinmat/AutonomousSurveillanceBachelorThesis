﻿#include "UavForRRT.h"
#include <string>

namespace App
{
	//Entity used in RRT and LinkedState. Uav used in State has other properties.

	UavForRRT::UavForRRT(const UavForRRT& other) : UavInterface(other)
	{
		//potřebuji naklonovat pouze polohu a rotaci, zbytek chci stejný
		currentGuidingPathPositions = other.currentGuidingPathPositions;	//předávám pointer na tu samou instanci, záměrně, aby se current_index posouval i starým stavům
		reachedGoal = other.reachedGoal;
		previousInput = other.previousInput;
	}

	UavForRRT::UavForRRT(shared_ptr<PointParticle> pointParticle) : 
		UavInterface(pointParticle), 
		currentGuidingPathPositions(make_shared<GuidingPathsCurrentPositions>())  //todo: do konstruktoru možná pøedávat délku cesty, abych mohl pole naalokovat hned na zaèátku.
	{
	}

	UavForRRT::UavForRRT(shared_ptr<Point> location, shared_ptr<Point> rotation) : 
		UavInterface(make_shared<PointParticle>(location, rotation)),
		currentGuidingPathPositions(make_shared<GuidingPathsCurrentPositions>())
	{
	}

	UavForRRT::UavForRRT(double locationX, double locationY, double rotationZ) : 
		UavInterface(make_shared<PointParticle>(locationX, locationY, rotationZ)),
		currentGuidingPathPositions(make_shared<GuidingPathsCurrentPositions>())
	{
	}

	UavForRRT::UavForRRT(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ) : 
		UavInterface(make_shared<PointParticle>(locationX, locationY, locationZ, rotationX, rotationY, rotationZ)),
		currentGuidingPathPositions(make_shared<GuidingPathsCurrentPositions>())
	{
	}

	UavForRRT::~UavForRRT()
	{
	}

	bool UavForRRT::isGoalReached() const
	{
		return reachedGoal != false;
	}

	shared_ptr<GoalInterface> UavForRRT::getReachedGoal() const
	{
		return reachedGoal;
	}

	void UavForRRT::setReachedGoal(shared_ptr<GoalInterface> reachedGoal)
	{
		this->reachedGoal = reachedGoal;
	}

	ostream& operator<<(ostream& os, const UavForRRT& obj)
	{
		return os << "id: " << obj.id << endl << "pointParticle: " << *obj.pointParticle;
	}

	shared_ptr<Goal> UavForRRT::getConcreteGoal()
	{
		return getReachedGoal()->getConcreteGoal(getPointParticle()->getLocation());
	}

	shared_ptr<GuidingPathsCurrentPositions> UavForRRT::getCurrentGuidingPathPositions() const
	{
		return currentGuidingPathPositions;
	}

	mObject UavForRRT::toJson() const
	{
		mObject object;
		object["id"] = this->id;
		object["pointParticle"] = this->pointParticle->toJson();
		object["previousInput"] = this->previousInput.toJson();
		return object;
	}

	shared_ptr<UavForRRT> UavForRRT::fromJson(mValue data)
	{
		auto pointParticleData = data.get_obj().at("pointParticle").get_obj();
		auto id = data.get_obj().at("id").get_int();
		auto previousInputData = data.get_obj().at("previousInput").get_obj();
		auto uav = make_shared<UavForRRT>(PointParticle::fromJson(pointParticleData));
		uav->id = id;
		uav->previousInput = CarLikeControl::fromJson(previousInputData);
		return uav;
	}

	CarLikeControl UavForRRT::getPreviousInput() const
	{
		return previousInput;
	}

	void UavForRRT::setPreviousInput(const CarLikeControl car_like_control)
	{
		previousInput = car_like_control;
	}

	void UavForRRT::setPreviousInputStep(double step)
	{
		previousInput.setStep(step);
	}

}