#include "CarLikeAnalyticMotionModel.h"
#include "Configuration.h"

namespace App
{

	CarLikeAnalyticMotionModel::CarLikeAnalyticMotionModel(shared_ptr<Configuration> configuration, shared_ptr<LoggerInterface> logger) : 
		configuration(configuration), logger(logger)
	{
	}

	CarLikeAnalyticMotionModel::~CarLikeAnalyticMotionModel()
	{
	}

	void CarLikeAnalyticMotionModel::calculateState(shared_ptr<UavInterface> state, CarLikeControl control)
	{
		double phi = state->getPointParticle()->getRotation()->getZ();

		double v = control.getStep();	//velocity
		double K = control.getTurn();	//curvature
		double w = 0;	//TODO: zmìnit, až to budu pøedìlávat na 3D
		double timeStep = configuration->getTimeStep();


		double diffX;
		double diffY;
		
		if(K == 0)
		{
			diffX = v*cos(phi)*timeStep;
			diffY = v*sin(phi)*timeStep;
		} else
		{
			diffX = (1 / K) * (sin(phi + K * v * timeStep) - sin(phi));
			diffY = - (1 / K) * (cos(phi + K * v * timeStep) - cos(phi));
		}

		state->getPointParticle()->getLocation()->changeX(diffX);
		state->getPointParticle()->getLocation()->changeY(diffY);
		state->getPointParticle()->getRotation()->changeZ(K * v * timeStep);
//		logger->logText("x: " + to_string(diffX) + ", y: " + to_string(diffY));
	}

	double CarLikeAnalyticMotionModel::getMinimalCurveRadius()
	{
		return 1 / configuration->getMaxTurn();
	}

	double CarLikeAnalyticMotionModel::getCurveRadius(shared_ptr<CarLikeControl> control)
	{
		return 1 / control->getTurn();
	}

	void CarLikeAnalyticMotionModel::setLogger(shared_ptr<LoggerInterface> logger)
	{
		this->logger = logger;
	}
}