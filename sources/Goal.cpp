#include "../include/Goal.h"

Goal::Goal(int x, int y, int height, int width)
{
	rectangle = new Rectangle(x, y, height, width);
}

Goal::~Goal() // destructor
{
	delete rectangle;
}