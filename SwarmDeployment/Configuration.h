#pragma once
#include "Core.h"
#include <QtCore/qdebug.h>

namespace App
{

	class Configuration
	{

	public:

		Configuration();
		int getAStarCellSize() const;
		int getMapNumber() const;
		int getUavCount() const;
		void setMapNumber(int mapNumber, QDebug debug);
		void setUavCount(int uavCount);
		void setCore(App::Core* core);
		int getWorldHeight() const;
		void setWorldHeight(int worldHeight);
		int getWorldWidth() const;
		void setWorldWidth(int worldWidth);
		double getUavSize() const;
		void setUavSize(double uav_size);

	protected:
		App::Core* core;	//if change in configuration happens from GUI, Core needs to be notified to call logger and pass changed Map to it.
		int aStarCellSize; // cell size for discretization of map for A*
		int mapNumber;
		int uavCount;
		int worldHeight;
		int worldWidth;
		double uavSize;
	};

}
