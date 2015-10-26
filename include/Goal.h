#pragma once
#include "Rectangle.h"

namespace App
{

	class Goal
	{

	public:
		Goal(int x, int y, int height, int width);
		~Goal();
		Rectangle *rectangle;

	};

}