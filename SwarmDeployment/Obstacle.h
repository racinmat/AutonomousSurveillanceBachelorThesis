#pragma once
#include "Rectangle.h"
#include "memory"

using namespace std;

namespace App
{

	class Obstacle
	{

	public:
		Obstacle(int x, int y, int width, int heigth);

		Obstacle(const Obstacle& other)
			: rectangle(make_shared<Rectangle>(*other.rectangle.get()))
		{
		}

		virtual ~Obstacle();
		shared_ptr<Rectangle> rectangle;
	};

}