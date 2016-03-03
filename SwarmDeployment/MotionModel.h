#pragma once
#include "CarLikeControl.h"
#include <memory>
#include "PointParticle.h"
#include "LoggerInterface.h"

namespace App
{

	class MotionModel
	{
	public:
		MotionModel();
		virtual void calculateState(shared_ptr<UavInterface> state, CarLikeControl control) = 0;
		virtual double getMinimalCurveRadius() = 0;	//calculates radius from max value from configuration
		virtual double getCurveRadius(shared_ptr<CarLikeControl> control) = 0;
		virtual ~MotionModel();
		virtual void setLogger(shared_ptr<LoggerInterface> logger) = 0;
	};

}

