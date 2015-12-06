#include "CarLikeControl.h"


namespace App
{

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

	std::ostream& operator<<(std::ostream& os, const CarLikeControl& obj)
	{
		return os
			<< "step: " << obj.step
			<< " turn: " << obj.turn;
	}
}