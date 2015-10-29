#include "PointParticle.h"


namespace App
{


	PointParticle::PointParticle(Point* location, Point* rotation)
	{
		this->location = location;
		this->rotation = rotation;
	}

	PointParticle::PointParticle(int locationX, int locationY, int rotationX, int rotationY)
	{
		this->location = new Point(locationX, locationY);
		this->rotation = new Point(rotationX, rotationY);
	}


	PointParticle::~PointParticle()
	{
	}

	Point* PointParticle::getLocation() const
	{
		return location;
	}

	void PointParticle::setLocation(Point* location)
	{
		this->location = location;
	}

	Point* PointParticle::getRotation() const
	{
		return rotation;
	}

	void PointParticle::setRotation(Point* rotation)
	{
		this->rotation = rotation;
	}
}