#include "Goal.h"

namespace App
{

	Goal::Goal(int x, int y, int width, int height)
	{
		rectangle = std::make_shared<Rectangle>(x, y, width, height);
	}

	Goal::~Goal() // destructor
	{
	}

}