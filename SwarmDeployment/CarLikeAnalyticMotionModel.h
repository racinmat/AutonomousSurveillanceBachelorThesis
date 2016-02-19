#pragma once
#include <memory>
#include "PointParticle.h"
#include "CarLikeControl.h"
#include "MotionModel.h"

using namespace std;

namespace App
{
	class Configuration;

	class CarLikeAnalyticMotionModel : public MotionModel
	{
	public:
		CarLikeAnalyticMotionModel(shared_ptr<Configuration> configuration);
		virtual ~CarLikeAnalyticMotionModel();
		void calculateState(shared_ptr<PointParticle> state, shared_ptr<CarLikeControl> control) override;
		double getMinimalCurveRadius() override;	//calculates radius from max value from configuration
		double getCurveRadius(shared_ptr<CarLikeControl> control) override;
	protected:
		shared_ptr<Configuration> configuration;
	};

}