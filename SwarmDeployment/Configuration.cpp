#include "Configuration.h"
#include <iostream>

namespace App
{

	Configuration::Configuration()
	{
		aStarCellSize = 50;
		mapNumber = 1;
		uavCount = 4;
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

	void Configuration::setMapNumber(int mapNumber, QDebug debug)
	{
		this->mapNumber = mapNumber;
		if (core != nullptr)
		{
			debug << "logging configuration change";
			core->logConfigurationChange();
		}
	}

	void Configuration::setUavCount(int uavCount)
	{
		this->uavCount = uavCount;
	}

	void Configuration::setCore(App::Core* core)
	{
		this->core = core;
	}

}