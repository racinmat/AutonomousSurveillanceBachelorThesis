#include "Rectangle.h"

namespace App
{

	Rectangle::Rectangle(int x, int y, int width, int height)
	{
		location = std::make_shared<Point>(x, y);
		m_height = height;
		m_width = width;
	}

	void Rectangle::setX(int x)
	{
		location->setX(x);
	}

	void Rectangle::setY(int y)
	{
		location->setY(y);
	}

	void Rectangle::setHeight(int height)
	{
		m_height = height;
	}

	void Rectangle::setWidth(int width)
	{
		m_width = width;
	}

	int Rectangle::getX() const
	{
		return location->getX();
	}

	int Rectangle::getY() const
	{
		return location->getY();
	}

	int Rectangle::getHeight() const
	{
		return m_height;
	}

	int Rectangle::getWidth() const
	{
		return m_width;
	}

}