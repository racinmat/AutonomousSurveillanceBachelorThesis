#pragma once
#include <memory>
#include "PointParticle.h"
#include "CarLikeControl.h"
#include "MotionModel.h"
#include "LoggerInterface.h"

using namespace std;

namespace App
{
	class Configuration;

	class CarLikeAnalyticMotionModel : public MotionModel
	{
	public:
		CarLikeAnalyticMotionModel(shared_ptr<Configuration> configuration, shared_ptr<LoggerInterface> logger);
		virtual ~CarLikeAnalyticMotionModel();
		void calculateState(shared_ptr<Uav> state, CarLikeControl control) override;
		double getMinimalCurveRadius() override;	//calculates radius from max value from configuration
		double getCurveRadius(shared_ptr<CarLikeControl> control) override;
		void setLogger(shared_ptr<LoggerInterface> logger) override;
	protected:
		shared_ptr<Configuration> configuration;
		shared_ptr<LoggerInterface> logger;
	};

}