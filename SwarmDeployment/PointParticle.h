#pragma once
#include "Point.h"
#include "memory"

using namespace std;

namespace App
{

	class PointParticle
	{
	public:
		PointParticle(shared_ptr<Point> location, shared_ptr<Point> rotation);
		PointParticle(int locationX, int locationY, int rotationX, int rotationY);
		~PointParticle();
		shared_ptr<Point> getLocation() const;
		void setLocation(shared_ptr<Point> location);
		shared_ptr<Point> getRotation() const;
		void setRotation(shared_ptr<Point> rotation);

	protected:
		shared_ptr<Point> location;
		shared_ptr<Point> rotation;
	};


}
