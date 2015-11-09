#pragma once
#include "Point.h"
#include <memory>

namespace App
{

	class Rectangle
	{
	public:
		Rectangle(int x, int y, int width, int height);

		int getX() const;
		int getY() const;
		int getHeight() const;
		int getWidth() const;

		void setX(int x);
		void setY(int y);
		void setHeight(int height);
		void setWidth(int width);

	protected:
		std::shared_ptr<Point> location;
		int m_height;
		int m_width;

	};

}