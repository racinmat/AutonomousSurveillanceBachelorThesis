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

	protected:
		App::Core* core;	//if change in configuration happens from GUI, Core needs to be notified to call logger and pass changed Map to it.
		int aStarCellSize = 50; // cell size for discretization of map for A*
		int mapNumber = 1;
		int uavCount = 3;
		int worldHeight = 1000;
		int worldWidth = 1000;
	};

}
