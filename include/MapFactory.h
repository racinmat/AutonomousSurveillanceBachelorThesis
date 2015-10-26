#pragma once
#include <vector>
#include "Map.h"

class MapFactory
{
public:
	std::vector<Map*> createMaps();
	
};
