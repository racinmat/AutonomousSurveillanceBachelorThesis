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
		virtual ~Obstacle();
		shared_ptr<Rectangle> rectangle;
	};

}