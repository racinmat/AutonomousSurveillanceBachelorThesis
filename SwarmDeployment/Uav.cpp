#include "Uav.h"
#include <string>
#include "UavForRRT.h"

namespace App
{
	Uav::Uav(const UavForRRT& other) : UavInterface(other)
	{
		previousInput = other.getPreviousInput();
	}

	Uav::Uav(const Uav& other) : UavInterface(other)
	{
		//potřebuji naklonovat pouze polohu a rotaci, zbytek chci stejný
		previousInput = other.previousInput;
	}

	Uav::Uav(shared_ptr<PointParticle> pointParticle) :
		UavInterface(pointParticle)
	{
	}


	Uav::Uav(shared_ptr<Point> location, shared_ptr<Point> rotation) : 
		UavInterface(make_shared<PointParticle>(location, rotation))
	{
	}

	Uav::Uav(double locationX, double locationY, double rotationZ) : 
		UavInterface(make_shared<PointParticle>(locationX, locationY, rotationZ))
	{
	}

	Uav::Uav(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ) : 
		UavInterface(make_shared<PointParticle>(locationX, locationY, locationZ, rotationX, rotationY, rotationZ))
	{
	}

	Uav::~Uav()
	{
	}

	ostream& operator<<(ostream& os, const Uav& obj)
	{
		return os << "id: " << obj.id << endl << "pointParticle: " << *obj.pointParticle;
	}

	mObject Uav::toJson() const
	{
		mObject object;
		object["id"] = this->id;
		object["pointParticle"] = this->pointParticle->toJson();
		object["previousInput"] = this->previousInput.toJson();
		return object;
	}

	shared_ptr<Uav> Uav::fromJson(mValue data)
	{
		auto pointParticleData = data.get_obj().at("pointParticle").get_obj();
		auto id = data.get_obj().at("id").get_int();
		auto previousInputData = data.get_obj().at("previousInput").get_obj();
		auto uav = make_shared<Uav>(PointParticle::fromJson(pointParticleData));
		uav->id = id;
		uav->previousInput = CarLikeControl::fromJson(previousInputData);
		return uav;
	}

	CarLikeControl Uav::getPreviousInput() const
	{
		return previousInput;
	}

	void Uav::setPreviousInput(const CarLikeControl car_like_control)
	{
		previousInput = car_like_control;
	}

	void Uav::setPreviousInputStep(double step)
	{
		previousInput.setStep(step);
	}

}
