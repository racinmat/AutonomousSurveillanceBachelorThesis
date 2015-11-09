#pragma once
#include "Rectangle.h"

namespace App
{

	class Obstacle
	{

	public:
		Obstacle(int x, int y, int width, int heigth);
		virtual ~Obstacle();
		App::Rectangle *rectangle;
	};

}