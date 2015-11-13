#include "PointParticle.h"


namespace App
{


	PointParticle::PointParticle(shared_ptr<Point> location, shared_ptr<Point> rotation)
	{
		this->location = location;
		this->rotation = rotation;
	}

	PointParticle::PointParticle(double locationX, double locationY, double rotationZ)
	{
		this->location = make_shared<Point>(locationX, locationY);
		this->rotation = make_shared<Point>(0, 0, rotationZ);
	}

	PointParticle::PointParticle(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ)
	{
		this->location = make_shared<Point>(locationX, locationY, locationZ);
		this->rotation = make_shared<Point>(rotationX, rotationY, rotationZ);
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
}