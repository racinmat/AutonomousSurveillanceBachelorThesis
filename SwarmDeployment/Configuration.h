#pragma once
#include "Core.h"

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
	};

}
