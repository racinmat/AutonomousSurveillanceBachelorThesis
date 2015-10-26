#pragma once
#include <vector>
#include "Map.h"

namespace App
{

	class MapFactory
	{
	public:
		std::vector<App::Map*> createMaps();

	};

}