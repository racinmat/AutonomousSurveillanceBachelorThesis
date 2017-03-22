#include "CarLikeControl.h"


namespace App
{
	CarLikeControl::CarLikeControl()
	{
		step = 0;
		turn = 0;
	}

	CarLikeControl::CarLikeControl(double step, double turn) : 
		step(step), turn(turn)
	{
	}

	CarLikeControl::~CarLikeControl()
	{
	}

	double CarLikeControl::getStep() const
	{
		return step;
	}

	double CarLikeControl::getTurn() const
	{
		return turn;
	}

	CarLikeControl::CarLikeControl(const CarLikeControl& other): 
		step(other.step), turn(other.turn)
	{
	}

	void CarLikeControl::setStep(const double step)
	{
		this->step = step;
	}

	Document CarLikeControl::toJson() const
	{
		Document d;
		d["step"] = this->step;
		d["turn"] = this->turn;
		return d;
	}

	CarLikeControl CarLikeControl::fromJson(Value data)
	{
		auto step = data["step"].GetDouble();
		auto turn = data["turn"].GetDouble();
		return CarLikeControl(step, turn);
	}

	std::ostream& operator<<(std::ostream& os, const CarLikeControl& obj)
	{
		return os
			<< "step: " << obj.step
			<< " turn: " << obj.turn;
	}
}