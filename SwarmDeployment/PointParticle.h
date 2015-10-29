#pragma once
#include "Point.h"

namespace App
{

	class PointParticle
	{
	public:
		PointParticle(Point* location, Point* rotation);
		PointParticle(int locationX, int locationY, int rotationX, int rotationY);
		~PointParticle();
		Point* getLocation() const;
		void setLocation(Point* location);
		Point* getRotation() const;
		void setRotation(Point* rotation);

	protected:
		Point* location;
		Point* rotation;
	};


}
