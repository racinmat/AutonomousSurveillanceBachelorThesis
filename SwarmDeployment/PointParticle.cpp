#include "PointParticle.h"


namespace App
{


	PointParticle::PointParticle(shared_ptr<Point> location, shared_ptr<Point> rotation)
	{
		this->location = location;
		this->rotation = rotation;
	}

	PointParticle::PointParticle(int locationX, int locationY, int rotationX, int rotationY)
	{
		this->location = make_shared<Point>(locationX, locationY);
		this->rotation = make_shared<Point>(rotationX, rotationY);
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