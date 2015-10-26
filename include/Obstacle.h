#pragma once
#include "Rectangle.h"

class Obstacle
{

public:
	Obstacle(int x, int y, int height, int width);
	Rectangle *rectangle;
};
