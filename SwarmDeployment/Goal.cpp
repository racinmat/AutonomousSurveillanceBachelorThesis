#include "Goal.h"
#include "Random.h"

namespace App
{

	Goal::Goal(int x, int y, int width, int height)
	{
		rectangle = std::make_shared<Rectangle>(x, y, width, height);
	}

	Goal::~Goal() // destructor
	{
	}

	bool Goal::contains(shared_ptr<Point> location)
	{
		return rectangle->contains(location);
	}

	shared_ptr<Point> Goal::getRandomPointInside()
	{
		int x = Random::inRange(rectangle->getX(), rectangle->getX() + rectangle->getWidth());
		int y = Random::inRange(rectangle->getY(), rectangle->getY() + rectangle->getHeight());
		return make_shared<Point>(x, y);
	}

	shared_ptr<Rectangle> Goal::getRectangle()
	{
		return rectangle;
	}

	bool operator==(const Goal& lhs, const Goal& rhs)
	{
		return *lhs.rectangle.get() == *rhs.rectangle.get();
	}

	bool operator!=(const Goal& lhs, const Goal& rhs)
	{
		return !(lhs == rhs);
	}

	std::size_t hash_value(const Goal& obj)
	{
		std::size_t seed = 0x2241B6D5;
		seed ^= (seed << 6) + (seed >> 2) + 0x2E9C86EF + hash_value(*obj.rectangle.get());
		return seed;
	}
}