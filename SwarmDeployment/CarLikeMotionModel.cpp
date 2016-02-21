#include "CarLikeMotionModel.h"
#include "Configuration.h"

#define PI 3.14159265358979323846

namespace App
{

	CarLikeMotionModel::CarLikeMotionModel(shared_ptr<Configuration> configuration, shared_ptr<LoggerInterface> logger) : 
		configuration(configuration), logger(logger)
	{
	}


	CarLikeMotionModel::~CarLikeMotionModel()
	{
	}

	void CarLikeMotionModel::calculateState(shared_ptr<PointParticle> state, shared_ptr<CarLikeControl> control)
	{
		double diffX = 0;
		double diffY = 0;

		double uav_size = configuration->getUavSize();
		double time_step = configuration->getTimeStep();
		double end_time = configuration->getEndTime();

		double dPhi = (control->getStep() / uav_size) * tan(control->getTurn());	//dPhi se nemìní v rámci vnitøního cyklu, takže staèí spošítat jen jednou

		for (double i = time_step; i < end_time; i += time_step)
		{
			//calculate derivatives from inputs
			double dx = control->getStep() * cos(state->getRotation()->getZ());	//pokud jsme ve 2D, pak jediná možná rotace je rotace okolo osy Z
			double dy = control->getStep() * sin(state->getRotation()->getZ());	//input není klasický bod se souøadnicemi X, Y, ale objekt se dvìma èísly, odpovídajícími dvìma vstupùm do car_like modelu

			//calculate current state variables
			diffX += dx * time_step;
			diffY += dy * time_step;
			state->getLocation()->changeX(dx * time_step);
			state->getLocation()->changeY(dy * time_step);
			state->getRotation()->changeZ(dPhi * time_step);
		}
		logger->logText("x: " + to_string(diffX) + ", y: " + to_string(diffY));
	}

	double CarLikeMotionModel::getCurveRadius(shared_ptr<CarLikeControl> control)
	{
		auto oldPoint = make_shared<PointParticle>(0, 0, 0);
		auto newPoint = make_shared<PointParticle>(*oldPoint.get());
		calculateState(newPoint, control);
		double pointsDistance = newPoint->getLocation()->getDistance(oldPoint->getLocation());
		//v geogebøe je nasimulováno spoèítání polomìru kruhu podle dat z modelu
		auto angle = getTotalRotation(control) / 2;
		double radius = (pointsDistance / 2) / cos(PI/2 - angle);
		return radius;
	}

	double CarLikeMotionModel::getMinimalCurveRadius()
	{
		auto maxControl = make_shared<CarLikeControl>(configuration->getDistanceOfNewNodes(), configuration->getMaxTurn());
		return getCurveRadius(maxControl);
	}

	double CarLikeMotionModel::getTotalRotation(shared_ptr<CarLikeControl> control)
	{
		double totalRotation = 0;
		double uav_size = configuration->getUavSize();
		double time_step = configuration->getTimeStep();
		double end_time = configuration->getEndTime();
		double dPhi = (control->getStep() / uav_size) * tan(control->getTurn());	//dPhi se nemìní v rámci vnitøního cyklu, takže staèí spošítat jen jednou
		for (double i = time_step; i < end_time; i += time_step)
		{
			totalRotation += dPhi;
		}
		totalRotation /= 20;	//nechápu, proè dìlit zrovna dvacítkou
		return totalRotation;
	}

	void CarLikeMotionModel::setLogger(shared_ptr<LoggerInterface> logger)
	{
		this->logger = logger;
	}
}