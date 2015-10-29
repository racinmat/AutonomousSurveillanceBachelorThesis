#include "Rectangle.h"

namespace App
{

	Rectangle::Rectangle(int x, int y, int height, int width)
	{
		m_x = x;
		m_y = y;
		m_height = height;
		m_width = width;
	}

	void Rectangle::setX(int x)
	{
		m_x = x;
	}

	void Rectangle::setY(int y)
	{
		m_y = y;
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
		return m_x;
	}

	int Rectangle::getY() const
	{
		return m_y;
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