#pragma once
#include "Rectangle.h"
#include "vector"

using namespace std;

namespace App
{

	class Goal
	{

	public:
		Goal(int x, int y, int width, int height);
		virtual ~Goal();
		std::shared_ptr<Rectangle> rectangle; //todo: pøesunout do private
		bool is_near(shared_ptr<Point> location);
	};

}