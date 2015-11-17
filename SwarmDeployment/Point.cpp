#include "Point.h"

namespace App
{

	Point::Point(double x, double y) : x(x), y(y), z(0)
	{
	}

	Point::Point(double x, double y, double z) : x(x), y(y), z(z)
	{
	}

	Point::Point(const Point& other) : x(other.x), y(other.y), z(other.z)
	{
	}

	Point::~Point()
	{
	}

	double Point::getX() const
	{
		return x;
	}

	void Point::setX(double x)
	{
		this->x = x;
	}

	double Point::getY() const
	{
		return y;
	}

	void Point::setY(double y)
	{
		this->y = y;
	}

	void Point::changeX(double x)
	{
		this->x += x;
	}

	void Point::changeY(double y)
	{
		this->y += y;
	}

	double Point::getZ() const
	{
		return z;
	}

	void Point::setZ(const double z)
	{
		this->z = z;
	}

	void Point::changeZ(double z)
	{
		this->z += z;
	}

	std::ostream& operator<<(std::ostream& os, const Point& obj)
	{
		return os
			<< "x: " << obj.x
			<< " y: " << obj.y
			<< " z: " << obj.z;
	}
}