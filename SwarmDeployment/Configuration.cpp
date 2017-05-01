#include "Configuration.h"
#include <iostream>

#define PI 3.14159265358979323846

namespace App
{

	Configuration::Configuration()
	{
		aStarCellSize = 50;
		mapNumber = 0;
		uavCount = 3;
		worldHeight = 1000;
		worldWidth = 1000;
//		worldHeight = 800;	//k mapě 8
//		worldWidth = 800;
		uavSize = 2.5;		//UAV is aapproximated by square, uavSize is length of one side of square
//		uavSize = 60;		//k mapě 8, měřeno na velkém UAV, délka strany. 
		samplingRadius = 60;		//poloměr kružnice, která je okolo bodu z A staru a okolo které se vybírá náhodný bod
		drawPeriod = 1;
		inputSamplesDist = 1;
		inputSamplesPhi = 3;
		rrtMinNodes = 1;
		rrtMaxNodes = 20000;
		nearCount = 1000;
		debug = true;
		distanceOfNewNodes = 20;	//k mapě 8, rychlost 20 cm/s
//		guidingNearDist = 40;		//max. vzdálenost od bodu z guidingPath, aby se použil další bod na cestě
		guidingNearDist = 38;		//pro PlacementMethod::Chain
//		guidingNearDist = 100;		//pro mapu 8, kde jsou velké vzdálenosti mezi 
		numberOfSolutions = 10000;
		guidedSamplingProbability = 1;
		nearestNeighborMethod = NNMethod::Total;
		maxTurn = 2 * tan(PI / 150);	//pro analytický model
//		maxTurn = 0.02;	//pro analytický model, mapa 8, poloměr křivosti 0.5 metru, tedy 50 cm
		timeStep = 1;
//		relativeDistanceMax = 80;
		relativeDistanceMax = 180;
		relativeDistanceMin = 24;
//		relativeDistanceMax = 500;		//pro mapu 8, 100 pixelů je 1 metr
//		relativeDistanceMin = 170;		//pro mapu 8
		localizationAngle = 2 * PI;
		requiredNeighbors = 1;
		checkFov = false;
		allowSwarmSplitting = false;	//nemá smysl mít true, protože pak ztrácím vlastnosti roje. pro celý roj jen jedna guiding path
		stop = false;
		textOutputEnabled = false;
		narrowPassageDivisor = 1;
		exitNarrowPassageTreshold = 1;
		narrowPassageCount = 0;
		divisionCount = 0;
		goalElementSize = 10;
		slowerMotionNearObstacles = false;
		obstacleIncrement = 30;			//virtuální zvětšení překážek
		maxSampleFrequency = 20;	//s 70Hz trval dubins příšerně dlouho, zvláště kvůli coldetectu, možná refactorovat použití coldetectu a zjistit, jak do něj narvat víc věcí najednou. Tím bych mohl volat 4x méně coldetectů
//		maxSampleCount = 2700;		//reálný počet na fyzickém UAV
		maxSampleCount = 10000000;	//nějaký vysoký počet, aby se limit neprojevil
		uavCameraX = 150;
		uavCameraY = 100;

		//standard method
		placementMethod = PlacementMethod::Standard;	//placement in AoI. Standard is one guiding path (or more paths, depending on allowSwarmSplitting), Chain is chain method.
		zeroStepEnabled = false;
		smartZeroStepEnabling = false;	//turns on zero step, when at least one UAV is near oblascle

		//chain method
//		placementMethod = PlacementMethod::Chain;	//placement in AoI. Standard is one guiding path (or more paths, depending on allowSwarmSplitting), Chain is chain method.
//		zeroStepEnabled = false;
//		smartZeroStepEnabling = true;	//turns on zero step, when at least one UAV is near oblascle

		smartZeroStepEnablingDistance = 50;
		maxFinalNodes = 300;

		if (relativeDistanceMin > relativeDistanceMax)
		{
			throw "Configuration is invalid, minimal relative distance is bigger than maximal relative distance.";
		}
		if (rrtMinNodes > rrtMaxNodes)
		{
			throw "Configuration is invalid, minimal rrt nodes count is bigger than maximal relative nodes count.";
		}
	}

	int Configuration::getAStarCellSize() const
	{
		return aStarCellSize;
	}

	int Configuration::getMapNumber() const
	{
		return mapNumber;
	}

	int Configuration::getUavCount() const
	{
		return uavCount;
	}

	void Configuration::setMapNumber(int mapNumber)
	{
		this->mapNumber = mapNumber;
		if (core)	//kontrola prázdného pointeru
		{
			core->logConfigurationChange();
		}
	}

	void Configuration::setUavCount(int uavCount)
	{
		this->uavCount = uavCount;
	}

	void Configuration::setCore(shared_ptr<Core> core)
	{
		this->core = core;
	}

	int Configuration::getWorldHeight() const
	{
		return worldHeight;
	}

	void Configuration::setWorldHeight(int worldHeight)
	{
		this->worldHeight = worldHeight;
	}

	int Configuration::getWorldWidth() const
	{
		return worldWidth;
	}

	void Configuration::setWorldWidth(int worldWidth)
	{
		this->worldWidth = worldWidth;
	}

	double Configuration::getUavSize() const
	{
		return uavSize;
	}

	void Configuration::setUavSize(double uav_size)
	{
		uavSize = uav_size;
	}

	double Configuration::getSamplingRadius() const
	{
		return samplingRadius;
	}

	int Configuration::getDrawPeriod() const
	{
		return drawPeriod;
	}

	int Configuration::getInputSamplesDist() const
	{
		return inputSamplesDist;
	}

	int Configuration::getInputSamplesPhi() const
	{
		return inputSamplesPhi;
	}

	int Configuration::getInputCount() const
	{
		int oneUavInputs = getInputSamplesDist() * getInputSamplesPhi();
		if (zeroStepEnabled)
		{
			oneUavInputs++;
		}
		auto total = pow(oneUavInputs, getUavCount());
		if (zeroStepEnabled)
		{
			total--;	//pokud je povolený nulový vstup, odeberu 1. kombinaci, jsou je nulový vstup pro všechna UAV najednou
		}
		return total;
	}

	int Configuration::getRrtMinNodes() const
	{
		return rrtMinNodes;
	}

	int Configuration::getRrtMaxNodes() const
	{
		return rrtMaxNodes;
	}

	int Configuration::getNearCount() const
	{
		return nearCount;
	}

	bool Configuration::getDebug() const
	{
		return debug;
	}

	double Configuration::getDistanceOfNewNodes() const
	{
		return distanceOfNewNodes;
	}

	double Configuration::getGuidingNearDist() const
	{
		return guidingNearDist;
	}

	int Configuration::getNumberOfSolutions() const
	{
		return numberOfSolutions;
	}

	double Configuration::getGuidedSamplingPropability() const
	{
		return guidedSamplingProbability;
	}

	NNMethod Configuration::getNearestNeighborMethod() const
	{
		return nearestNeighborMethod;
	}

	double Configuration::getMaxTurn() const
	{
		return maxTurn;
	}

	double Configuration::getTimeStep() const
	{
		return timeStep;
	}

	double Configuration::getRelativeDistanceMin() const
	{
		return relativeDistanceMin;
	}

	double Configuration::getRelativeDistanceMax() const
	{
		return relativeDistanceMax;
	}

	double Configuration::getLocalizationAngle() const
	{
		return localizationAngle;
	}

	int Configuration::getRequiredNeighbors() const
	{
		return requiredNeighbors;
	}

	bool Configuration::getAllowSwarmSplitting() const
	{
		return allowSwarmSplitting;
	}

	bool Configuration::getCheckFov() const
	{
		return checkFov;
	}

	bool Configuration::getStop() const
	{
		return stop;
	}

	void Configuration::setStop(bool stop)
	{
		this->stop = stop;
	}

	bool Configuration::isTextOutputEnabled() const
	{
		return textOutputEnabled;
	}

	void Configuration::inNarrowPassage()
	{
		distanceOfNewNodes /= narrowPassageDivisor;
		maxTurn *= narrowPassageDivisor;
		guidingNearDist /= narrowPassageDivisor;
		narrowPassageCount = 0;
		divisionCount++;
	}

	void Configuration::outsideNarrowPassage()
	{
		narrowPassageCount++;
		//Far from obstacle
		if (narrowPassageCount > exitNarrowPassageTreshold)	//usv jsou daleko od překážek, navrácení původních hodnot
		{
			distanceOfNewNodes *= pow(narrowPassageDivisor, divisionCount);
			maxTurn /= pow(narrowPassageDivisor, divisionCount);
			guidingNearDist *= pow(narrowPassageDivisor, divisionCount);
			narrowPassageCount = 0;
			divisionCount = 0;
		}
	}

	int Configuration::getGoalElementSize() const
	{
		return goalElementSize;
	}

	bool Configuration::isSlowerMotionNearObstacles() const
	{
		return slowerMotionNearObstacles;
	}

	int Configuration::getObstacleIncrement() const
	{
		return obstacleIncrement;
	}

	int Configuration::getMaxSampleFrequency() const
	{
		return maxSampleFrequency;
	}

	int Configuration::getMaxSampleCount() const
	{
		return maxSampleCount;
	}

	int Configuration::getUavCameraX() const
	{
		return uavCameraX;
	}

	int Configuration::getUavCameraY() const
	{
		return uavCameraY;
	}

	PlacementMethod Configuration::getPlacementMethod() const
	{
		return placementMethod;
	}

	bool Configuration::getZeroStepEnabled() const
	{
		return zeroStepEnabled;
	}

	bool Configuration::getSmartZeroStepEnabling() const
	{
		return smartZeroStepEnabling;
	}

	int Configuration::getSmartZeroStepEnablingDistance() const
	{
		return smartZeroStepEnablingDistance;
	}

	void Configuration::setZeroStepEnabled(const bool zero_step_enabled)
	{
		zeroStepEnabled = zero_step_enabled;
	}

	void Configuration::setMaxSampleFrequency(const int max_sample_frequency)
	{
		maxSampleFrequency = max_sample_frequency;
	}

	int Configuration::getMaxFinalNodes() const
	{
		return maxFinalNodes;
	}
}
