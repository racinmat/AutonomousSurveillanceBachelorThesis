#include "Uav.h"
#include <string>

namespace App
{
	int Uav::lastId = 0;

	Uav::Uav(const Uav& other) 
	{
		pointParticle = make_shared<PointParticle>(*other.pointParticle.get());	//potøebuji naklonovat pouze polohu a rotaci, zbytek chci stejný
		currentGuidingPathPositions = other.currentGuidingPathPositions;	//pøedávám pointer na tu samou instanci, zámìrnì, aby se current_index posouval i starým stavùm
		reachedGoal = other.reachedGoal;
		previousInput = other.previousInput;
		id = other.id;
	}

	Uav::Uav(shared_ptr<PointParticle> pointParticle) : 
		currentGuidingPathPositions(make_shared<GuidingPathsCurrentPositions>()), pointParticle(pointParticle), id(lastId++)	//todo: do konstruktoru možná pøedávat délku cesty, abych mohl pole naalokovat hned na zaèátku.
	{
	}

	Uav::Uav(shared_ptr<Point> location, shared_ptr<Point> rotation) : 
		currentGuidingPathPositions(make_shared<GuidingPathsCurrentPositions>()), pointParticle(make_shared<PointParticle>(location, rotation)), id(lastId++)
	{
	}

	Uav::Uav(double locationX, double locationY, double rotationZ) : 
		currentGuidingPathPositions(make_shared<GuidingPathsCurrentPositions>()), pointParticle(make_shared<PointParticle>(locationX, locationY, rotationZ)), id(lastId++)
	{
	}

	Uav::Uav(double locationX, double locationY, double locationZ, double rotationX, double rotationY, double rotationZ) :
		currentGuidingPathPositions(make_shared<GuidingPathsCurrentPositions>()), 
		pointParticle(make_shared<PointParticle>(locationX, locationY, locationZ, rotationX, rotationY, rotationZ)), id(lastId++)
	{
	}

	Uav::~Uav()
	{
	}

	shared_ptr<PointParticle> Uav::getPointParticle() const
	{
		return pointParticle;
	}

	bool Uav::isGoalReached() const
	{
		return reachedGoal != false;
	}

	shared_ptr<GoalInterface> Uav::getReachedGoal() const
	{
		return reachedGoal;
	}

	void Uav::setReachedGoal(shared_ptr<GoalInterface> reachedGoal)
	{
		this->reachedGoal = reachedGoal;
	}

	ostream& operator<<(ostream& os, const Uav& obj)
	{
		return os << "id: " << obj.id << endl << "pointParticle: " << *obj.pointParticle;
	}

	bool operator<(const Uav& lhs, const Uav& rhs)
	{
		return lhs.id < rhs.id;
	}

	bool operator<=(const Uav& lhs, const Uav& rhs)
	{
		return !(rhs < lhs);
	}

	bool operator>(const Uav& lhs, const Uav& rhs)
	{
		return rhs < lhs;
	}

	bool operator>=(const Uav& lhs, const Uav& rhs)
	{
		return !(lhs < rhs);
	}

	bool operator==(const Uav& lhs, const Uav& rhs)
	{
		return lhs.id == rhs.id;
	}

	bool operator!=(const Uav& lhs, const Uav& rhs)
	{
		return !(lhs == rhs);
	}

	size_t Uav::hash_value() const
	{
		size_t seed = 0x28003F72;
		seed ^= (seed << 6) + (seed >> 2) + 0x1B543A89 + static_cast<size_t>(id);
		return seed;
	}

	int Uav::getId() const
	{
		return id;
	}

	shared_ptr<Goal> Uav::getConcreteGoal()
	{
		return getReachedGoal()->getConcreteGoal(getPointParticle()->getLocation());
	}

	shared_ptr<GuidingPathsCurrentPositions> Uav::getCurrentGuidingPathPositions() const
	{
		return currentGuidingPathPositions;
	}

	void Uav::setId(const int id)
	{
		this->id = id;
	}

	void Uav::setPointParticle(const shared_ptr<PointParticle> point_particle)
	{
		pointParticle = point_particle;
	}

	mObject Uav::toJson() const
	{
		mObject object;
		object["id"] = this->id;
		object["pointParticle"] = this->pointParticle->toJson();
		object["previousInput"] = this->previousInput.toJson();
		return object;
	}

	shared_ptr<Uav> Uav::fromJson(mValue data)
	{
		auto pointParticleData = data.get_obj().at("pointParticle").get_obj();
		auto id = data.get_obj().at("id").get_int();
		auto previousInputData = data.get_obj().at("previousInput").get_obj();
		auto uav = make_shared<Uav>(PointParticle::fromJson(pointParticleData));
		uav->id = id;
		uav->previousInput = CarLikeControl::fromJson(previousInputData);
		return uav;
	}

	CarLikeControl Uav::getPreviousInput() const
	{
		return previousInput;
	}

	void Uav::setPreviousInput(const CarLikeControl car_like_control)
	{
		previousInput = car_like_control;
	}

	void Uav::setPreviousInputStep(double step)
	{
		previousInput.setStep(step);
	}

}
