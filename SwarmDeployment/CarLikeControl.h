#pragma once
#include <ostream>
#include <json_spirit/json_spirit_reader.h>

using namespace json_spirit;

namespace App
{

	class CarLikeControl
	{
	public:

		CarLikeControl();
		CarLikeControl(double step, double turn);
		CarLikeControl(const CarLikeControl& other);
		virtual ~CarLikeControl();
		virtual double getStep() const;
		virtual double getTurn() const;
		friend std::ostream& operator<<(std::ostream& os, const CarLikeControl& obj);
		virtual void setStep(const double step);
		mObject toJson() const;
		static CarLikeControl fromJson(mValue data);

	protected:
		double step;	//krok vp�ed
		double turn;	//oto�ka
	};

}

