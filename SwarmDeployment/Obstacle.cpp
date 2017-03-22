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

	shared_ptr<Obstacle> Obstacle::fromJson(Value& data)
	{
		auto height = data["height"].GetInt();
		auto width = data["width"].GetInt();
		auto &location = data["location"];
		auto x = location["x"].GetInt();
		auto y = location["y"].GetInt();
		return make_shared<Obstacle>(x, y, width, height);
	}

	Obstacle::~Obstacle()
	{
	}
}