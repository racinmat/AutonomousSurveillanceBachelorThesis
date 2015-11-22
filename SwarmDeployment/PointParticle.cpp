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

	std::ostream& operator<<(std::ostream& os, const PointParticle& obj)
	{
		return os
			<< "location: " << *obj.location.get() << endl
			<< " rotation: " << *obj.rotation.get();
	}
}