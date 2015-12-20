#include "Rectangle.h"

namespace App
{

	Rectangle::Rectangle(int x, int y, int width, int height)
	{
		location = make_shared<Point>(x, y);
		m_height = height;
		m_width = width;
	}

	Rectangle::Rectangle(const Rectangle& other): 
		location(make_shared<Point>(*other.location.get())),
	    m_height(other.m_height),
	    m_width(other.m_width)
	{
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

	bool Rectangle::contains(shared_ptr<Point> point)
	{
		return point->getX() >= getX() && point->getX() <= (getX() + getWidth()) && point->getY() >= getY() && point->getY() <= (getY() + getHeight());
	}

	double Rectangle::getVolume() const
	{
		return getHeight() * getWidth();
	}

	shared_ptr<Point> Rectangle::getMiddle()
	{
		return make_shared<Point>(getX() + getWidth()/2, getY() + getHeight()/2);
	}

	mObject Rectangle::toJson() const
	{
		mObject object;
		object["location"] = this->location->toJson();
		object["width"] = this->m_width;
		object["height"] = this->m_height;
		return object;
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

	bool operator==(const Rectangle& lhs, const Rectangle& rhs)
	{
		return lhs.location == rhs.location
			&& lhs.m_height == rhs.m_height
			&& lhs.m_width == rhs.m_width;
	}

	bool operator!=(const Rectangle& lhs, const Rectangle& rhs)
	{
		return !(lhs == rhs);
	}

	size_t hash_value(const Rectangle& obj)
	{
		size_t seed = 0x686FF752;
		seed ^= (seed << 6) + (seed >> 2) + 0x6F4193A6 + hash_value(*obj.location.get());
		seed ^= (seed << 6) + (seed >> 2) + 0x528663F5 + static_cast<size_t>(obj.m_height);
		seed ^= (seed << 6) + (seed >> 2) + 0x4AC2A83C + static_cast<size_t>(obj.m_width);
		return seed;
	}
}