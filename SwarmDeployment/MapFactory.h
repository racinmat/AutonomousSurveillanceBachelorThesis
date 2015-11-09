#pragma once
#include <vector>
#include "Map.h"
#include <memory>

namespace App
{

	class MapFactory
	{
	public:
		std::vector<std::shared_ptr<Map>> createMaps(int uavCount);

	};

}