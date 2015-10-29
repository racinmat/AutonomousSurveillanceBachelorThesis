#pragma once
#include "Map.h"

namespace App
{

	class LoggerInterface
	{
	public:
		LoggerInterface();
		~LoggerInterface();
		virtual void logSelectedMap(Map* map, int worldWidth, int worldHeight);
	};

}