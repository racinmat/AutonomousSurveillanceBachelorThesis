#pragma once
#include "Core.h"
#include "Enums.h"

using namespace std;

namespace App
{

	class Configuration
	{

	public:

		Configuration();
		virtual int getAStarCellSize() const;
		virtual int getMapNumber() const;
		virtual int getUavCount() const;
		virtual void setMapNumber(int mapNumber);
		virtual void setUavCount(int uavCount);
		virtual void setCore(shared_ptr<Core> core);
		virtual int getWorldHeight() const;
		virtual void setWorldHeight(int worldHeight);
		virtual int getWorldWidth() const;
		virtual void setWorldWidth(int worldWidth);
		virtual double getUavSize() const;
		virtual void setUavSize(double uav_size);
		virtual double getSamplingRadius() const;
		virtual int getDrawPeriod() const;
		virtual int getInputSamplesDist() const;
		virtual int getInputSamplesPhi() const;
		virtual int getInputCount() const;
		virtual int getRrtMinNodes() const;
		virtual int getRrtMaxNodes() const;
		virtual int getNearCount() const;
		virtual bool getDebug() const;
		virtual double getDistanceOfNewNodes() const;
		virtual double getGuidingNearDist() const;
		virtual int getNumberOfSolutions() const;
		virtual double getGuidedSamplingPropability() const;
		virtual NNMethod getNearestNeighborMethod() const;
		virtual double getMaxTurn() const;
		virtual double getTimeStep() const;
		virtual double getRelativeDistanceMin() const;
		virtual double getRelativeDistanceMax() const;
		virtual double getLocalizationAngle() const;
		virtual int getRequiredNeighbors() const;
		virtual bool getAllowSwarmSplitting() const;
		virtual bool getCheckFov() const;
		virtual bool getStop() const;
		virtual void setStop(bool stop);
		virtual bool isTextOutputEnabled() const;
		virtual void inNarrowPassage();
		virtual void outsideNarrowPassage();
		virtual int getGoalElementSize() const;
		virtual bool isSlowerMotionNearObstacles() const;
		virtual int getObstacleIncrement() const;
		virtual int getMaxSampleFrequency() const;
		virtual int getMaxSampleCount() const;

	protected:
		shared_ptr<Core> core;	//if change in configuration happens from GUI, Core needs to be notified to call logger and pass changed Map to it.
		int aStarCellSize; // cell size for discretization of map for A*
		int mapNumber;
		int uavCount;
		int worldHeight;
		int worldWidth;
		double uavSize;
		double samplingRadius;
		int drawPeriod;
		int inputSamplesDist;
		int inputSamplesPhi;
		int rrtMinNodes;
		int rrtMaxNodes;
		int nearCount;
		bool debug;
		int numberOfSolutions;
		double guidedSamplingPropability;
		NNMethod nearestNeighborMethod;
		double timeStep;
		double relativeDistanceMin;
		double relativeDistanceMax;
		double localizationAngle;
		int requiredNeighbors;
		bool allowSwarmSplitting;
		bool checkFov;
		bool stop;
		bool textOutputEnabled;
		int narrowPassageDivisor;
		int exitNarrowPassageTreshold;
		int narrowPassageCount;
		double distanceOfNewNodes;
		double guidingNearDist;
		double maxTurn;
		int divisionCount;
		int goalElementSize;
		bool slowerMotionNearObstacles;
		int obstacleIncrement;
		int maxSampleFrequency;		//used for resampling of trajectories before exporting to csv, max frequency UAV is able to follow
		int maxSampleCount;			//max count of samples that fit to UAV internal memory
	};

}
