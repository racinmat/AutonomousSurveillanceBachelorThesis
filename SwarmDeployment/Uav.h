#pragma once
#include "PointParticle.h"

namespace App
{

	class Uav
	{
	public:
		Uav(const Uav& other);
		Uav(shared_ptr<PointParticle> pointParticle);
		Uav(shared_ptr<Point> location, shared_ptr<Point> rotation);
		Uav(double locationX, double locationY, double rotationZ);
		Uav(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ);
		virtual ~Uav();
		virtual shared_ptr<PointParticle> getPointParticle() const;
		friend ostream& operator<<(ostream& os, const Uav& obj);

	protected:
		shared_ptr<PointParticle> pointParticle;
	};

}
