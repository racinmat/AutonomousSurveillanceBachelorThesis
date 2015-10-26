#pragma once
#include "Rectangle.h"

namespace App
{

	class Obstacle
	{

	public:
		Obstacle(int x, int y, int height, int width);
		App::Rectangle *rectangle;
	};

}