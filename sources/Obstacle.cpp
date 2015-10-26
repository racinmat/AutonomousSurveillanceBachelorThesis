#include "../include/Obstacle.h"

namespace App
{

	Obstacle::Obstacle(int x, int y, int height, int width)
	{
		rectangle = new Rectangle(x, y, height, width);
	}

}