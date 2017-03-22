#pragma once
#include "PointParticle.h"
#include <vector>
#include "Goal.h"
#include "GuidingPathsCurrentPositions.h"
#include "CarLikeControl.h"
#include "UavInterface.h"


namespace App
{

	class Uav : public UavInterface
	{
	public:
		explicit Uav(const UavForRRT& other);
		Uav(const Uav& other);
		explicit Uav(shared_ptr<PointParticle> pointParticle);
		explicit Uav(shared_ptr<Point> location, shared_ptr<Point> rotation);
		explicit Uav(double locationX, double locationY, double rotationZ);
		explicit Uav(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ);
		virtual ~Uav();
		friend ostream& operator<<(ostream& os, const Uav& obj);
		Value toJson(Document& d) const;
		static shared_ptr<Uav> fromJson(Value& data);
		virtual CarLikeControl getPreviousInput() const;
		virtual void setPreviousInput(const CarLikeControl car_like_control);
		virtual void setPreviousInputStep(double step);

	protected:
		CarLikeControl previousInput;
	};


}
