﻿#include "Configuration.h"
#include <iostream>

namespace App
{

	Configuration::Configuration()
	{
		aStarCellSize = 50;
		mapNumber = 1;
		uavCount = 3;
		worldHeight = 1000;
		worldWidth = 1000;
		uavSize = 0.5;

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
}