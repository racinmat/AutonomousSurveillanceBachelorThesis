#pragma once

namespace App
{

	class Configuration
	{
	public:
		int aStarCellSize = 50; // cell size for discretization of map for A*
		int mapNumber = 1;
		int uavCount = 3;
	};

}
