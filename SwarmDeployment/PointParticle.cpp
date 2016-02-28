#include "PointParticle.h"
#include <ostream>


namespace App
{


	PointParticle::PointParticle(shared_ptr<Point> location, shared_ptr<Point> rotation) : location(location), rotation(rotation)
	{
	}

	PointParticle::PointParticle(double locationX, double locationY, double rotationZ) : 
		location(make_shared<Point>(locationX, locationY)),
		rotation(make_shared<Point>(0, 0, rotationZ))
	{
	}

	PointParticle::PointParticle(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ) : 
		location(make_shared<Point>(locationX, locationY, locationZ)),
		rotation(make_shared<Point>(rotationX, rotationY, rotationZ))
	{
	}

	PointParticle::PointParticle(const PointParticle& other) : 
		location(make_shared<Point>(*other.location.get())), 
		rotation(make_shared<Point>(*other.rotation.get()))
	{
	}

	PointParticle::~PointParticle()
	{
	}

	shared_ptr<Point> PointParticle::getLocation() const
	{
		return location;
	}

	void PointParticle::setLocation(shared_ptr<Point> location)
	{
		this->location = location;
	}

	shared_ptr<Point> PointParticle::getRotation() const
	{
		return rotation;
	}

	void PointParticle::setRotation(shared_ptr<Point> rotation)
	{
		this->rotation = rotation;
	}

	geom::Position PointParticle::toPosition()
	{
		return geom::Position(location->toGeomPoint(), rotation->getZ());
	}

	mObject PointParticle::toJson() const
	{
		mObject object;
		object["location"] = this->location->toJson();
		object["rotation"] = this->rotation->toJson();
		return object;
	}

	shared_ptr<PointParticle> PointParticle::fromJson(mObject data)
	{
		auto location = data.at("location");
		auto x = location.get_obj().at("x").get_real();
		auto y = location.get_obj().at("y").get_real();
		auto rotation = data.at("rotation");
		auto z = location.get_obj().at("z").get_real();
		return make_shared<PointParticle>(x, y, z);
	}

	std::ostream& operator<<(std::ostream& os, const PointParticle& obj)
	{
		return os
			<< "location: " << *obj.location.get() << endl
			<< " rotation: " << *obj.rotation.get();
	}

	bool operator==(const PointParticle& lhs, const PointParticle& rhs)
	{
		return *lhs.location.get() == *rhs.location.get()
			&& *lhs.rotation.get() == *rhs.rotation.get();
	}

	bool operator!=(const PointParticle& lhs, const PointParticle& rhs)
	{
		return !(lhs == rhs);
	}
}