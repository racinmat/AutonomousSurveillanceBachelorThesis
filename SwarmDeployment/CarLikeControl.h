#pragma once
#include <ostream>

namespace App
{

	class CarLikeControl
	{
	public:
		CarLikeControl(double step, double turn);
		virtual ~CarLikeControl();
		virtual double getStep() const;
		virtual double getTurn() const;
		friend std::ostream& operator<<(std::ostream& os, const CarLikeControl& obj);
		CarLikeControl(const CarLikeControl& other);

	protected:
		double step;	//krok vpøed
		double turn;	//otoèka
	};

}

