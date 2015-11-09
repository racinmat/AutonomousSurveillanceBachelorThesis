#include "Obstacle.h"

namespace App
{

	Obstacle::Obstacle(int x, int y, int width, int height)
	{
		rectangle = make_shared<Rectangle>(x, y, width, height);
	}

	Obstacle::~Obstacle()
	{
	}
}