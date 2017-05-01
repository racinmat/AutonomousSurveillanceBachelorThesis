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

	Value Uav::toJson(Document::AllocatorType& allocator) const
	{
		Value object(kObjectType);
		object.AddMember("id", this->id, allocator);
		object.AddMember("pointParticle", this->pointParticle->toJson(allocator), allocator);
		object.AddMember("previousInput", this->previousInput.toJson(allocator), allocator);
		return object;
	}

	shared_ptr<Uav> Uav::fromJson(Value& data)
	{
		auto pointParticleData = data["pointParticle"].GetObject();
		auto id = data["id"].GetInt();
		auto previousInputData = data["previousInput"].GetObject();
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
