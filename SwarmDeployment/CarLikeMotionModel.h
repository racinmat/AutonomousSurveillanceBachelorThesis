#pragma once
#include <memory>
#include "PointParticle.h"
#include "CarLikeControl.h"

using namespace std;

namespace App
{
	class Configuration;

	class CarLikeMotionModel
	{
	public:
		CarLikeMotionModel(shared_ptr<Configuration> configuration);
		virtual ~CarLikeMotionModel();
		void calculateState(shared_ptr<PointParticle> state, shared_ptr<CarLikeControl> control);
		double getCurveRadius(shared_ptr<CarLikeControl> control);
		double getMinimalCurveRadius();	//calculates radius from max value from configuration
	protected:
		shared_ptr<Configuration> configuration;
		double getTotalRotation(shared_ptr<CarLikeControl> control);
	};

}