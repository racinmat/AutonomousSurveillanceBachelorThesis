#pragma once
#include <ostream>
#include "rapidjson/document.h"

using namespace rapidjson;

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
		Value toJson(Document::AllocatorType& allocator) const;
		static CarLikeControl fromJson(Value data);

	protected:
		double step;	//krok vp�ed
		double turn;	//oto�ka
	};

}

