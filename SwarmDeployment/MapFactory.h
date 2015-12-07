#pragma once
#include <vector>
#include "Map.h"
#include <memory>

using namespace std;

namespace App
{

	class MapFactory
	{
	public:
		vector<shared_ptr<Map>> createMaps(int uavCount);

	};

}