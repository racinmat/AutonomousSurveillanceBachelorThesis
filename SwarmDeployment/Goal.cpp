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

	bool Goal::is_near(shared_ptr<Point> location)
	{
		return rectangle->contains(location);
	}
}