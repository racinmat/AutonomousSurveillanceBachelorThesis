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
		PointParticle(double locationX, double locationY, double rotationZ);
		PointParticle(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ);
		PointParticle(const PointParticle& other);
		~PointParticle();
		shared_ptr<Point> getLocation() const;
		void setLocation(shared_ptr<Point> location);
		shared_ptr<Point> getRotation() const;
		void setRotation(shared_ptr<Point> rotation);
		friend std::ostream& operator<<(std::ostream& os, const PointParticle& obj);

	protected:
		shared_ptr<Point> location;
		shared_ptr<Point> rotation;
	};


}
