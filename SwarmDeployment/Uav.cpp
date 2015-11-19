#include "Uav.h"


namespace App
{
	Uav::Uav(const Uav& other) 
	{
		pointParticle = make_shared<PointParticle>(*other.pointParticle.get());
		current_index = other.current_index;
		reachedGoal = other.reachedGoal;
	}

	Uav::Uav(shared_ptr<PointParticle> pointParticle) : 
		pointParticle(pointParticle)
	{
	}

	Uav::Uav(shared_ptr<Point> location, shared_ptr<Point> rotation) : 
		pointParticle(make_shared<PointParticle>(location, rotation))
	{
	}

	Uav::Uav(double locationX, double locationY, double rotationZ) : 
		pointParticle(make_shared<PointParticle>(locationX, locationY, rotationZ))
	{
	}

	Uav::Uav(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ) :
		pointParticle(make_shared<PointParticle>(locationX, locationY, locationZ, rotationX, rotationY, rotationZ))
	{
	}

	Uav::~Uav()
	{
	}

	shared_ptr<App::PointParticle> Uav::getPointParticle() const
	{
		return pointParticle;
	}

	bool Uav::isGoalReached() const
	{
		return reachedGoal != false;
	}

	shared_ptr<Goal> Uav::getReachedGoal() const
	{
		return reachedGoal;
	}

	void Uav::setReachedGoal(shared_ptr<Goal> reachedGoal)
	{
		this->reachedGoal = reachedGoal;
	}

	std::ostream& operator<<(std::ostream& os, const Uav& obj)
	{
		return os << "pointParticle: " << obj.pointParticle;
	}
}
