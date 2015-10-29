#include "Goal.h"

namespace App
{

	Goal::Goal(int x, int y, int height, int width)
	{
		rectangle = new Rectangle(x, y, height, width);
	}

	Goal::~Goal() // destructor
	{
		delete rectangle;
	}

}