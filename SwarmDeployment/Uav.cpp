#include "Uav.h"


namespace App
{
	int Uav::lastId = 0;

	Uav::Uav(const Uav& other) 
	{
		pointParticle = make_shared<PointParticle>(*other.pointParticle.get());	//potøebuji naklonovat pouze polohu a rotaci, zbytek chci stejný
		current_indexes = other.current_indexes;	//pøedávám pointer na tu samou instanci, zámìrnì, aby se current_index posouval i starým stavùm
		reachedGoal = other.reachedGoal;
		id = other.id;
	}

	Uav::Uav(shared_ptr<PointParticle> pointParticle) : 
		current_indexes(make_shared<GuidingPathsCurrentPositions>()), pointParticle(pointParticle), id(lastId++)	//todo: do konstruktoru možná pøedávat délku cesty, abych mohl pole naalokovat hned na zaèátku.
	{
	}

	Uav::Uav(shared_ptr<Point> location, shared_ptr<Point> rotation) : 
		current_indexes(make_shared<GuidingPathsCurrentPositions>()), pointParticle(make_shared<PointParticle>(location, rotation)), id(lastId++)
	{
	}

	Uav::Uav(double locationX, double locationY, double rotationZ) : 
		current_indexes(make_shared<GuidingPathsCurrentPositions>()), pointParticle(make_shared<PointParticle>(locationX, locationY, rotationZ)), id(lastId++)
	{
	}

	Uav::Uav(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ) :
		current_indexes(make_shared<GuidingPathsCurrentPositions>()), pointParticle(make_shared<PointParticle>(locationX, locationY, locationZ, rotationX, rotationY, rotationZ)), id(lastId++)
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
