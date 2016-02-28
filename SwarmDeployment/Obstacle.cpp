#include "Obstacle.h"

namespace App
{

	Obstacle::Obstacle(int x, int y, int width, int height)
	{
		rectangle = make_shared<Rectangle>(x, y, width, height);
	}

	Obstacle::Obstacle(const Obstacle& other): rectangle(make_shared<Rectangle>(*other.rectangle.get()))
	{
	}

	shared_ptr<Obstacle> Obstacle::fromJson(mValue data)
	{
		auto height = data.get_obj().at("height").get_int();
		auto width = data.get_obj().at("width").get_int();
		auto location = data.get_obj().at("location");
		auto x = location.get_obj().at("x").get_int();
		auto y = location.get_obj().at("y").get_int();
		return make_shared<Obstacle>(x, y, width, height);
	}

	Obstacle::~Obstacle()
	{
	}
}