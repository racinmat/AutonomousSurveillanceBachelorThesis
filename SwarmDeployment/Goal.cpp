#include "Goal.h"

namespace App
{

	Goal::Goal(int x, int y, int width, int height)
	{
		rectangle = new Rectangle(x, y, width, height);
	}

	Goal::~Goal() // destructor
	{
		delete rectangle;
	}

}