#include "Point.h"
#include <string>

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

	string Point::toString()
	{
		return 
			  " x: " + to_string(x)
			+ " y: " + to_string(y)
			+ " z: " + to_string(z);

	}

	double Point::getDistance(std::shared_ptr<Point> point)
	{
		return sqrt(getDistanceSquared(point));
	}

	double Point::getDistanceSquared(std::shared_ptr<Point> point)
	{
		return pow(fabs(getX() - point->getX()), 2)
			+ pow(fabs(getY() - point->getY()), 2);
	}

	void Point::moveBy(shared_ptr<Point> point)
	{
		moveBy(*point.get());
	}

	void Point::moveBy(Point point)
	{
		moveBy(point.getX(), point.getY(), point.getZ());
	}

	void Point::moveBy(double x, double y, double z)
	{
		changeX(x);
		changeY(y);
		changeZ(z);
	}

	geom::Point Point::toGeomPoint()
	{
		return geom::Point(x, y);
	}

	mObject Point::toJson() const
	{
		mObject object;
		object["x"] = this->x;
		object["y"] = this->y;
		object["z"] = this->z;
		return object;
	}

	std::ostream& operator<<(std::ostream& os, const Point& obj)
	{
		return os
			<< " x: " << obj.x
			<< " y: " << obj.y
			<< " z: " << obj.z;
	}

	bool operator==(const Point& lhs, const Point& rhs)
	{
		return fabs(lhs.x - rhs.x) < DBL_EPSILON
			&& fabs(lhs.y - rhs.y) < DBL_EPSILON
			&& fabs(lhs.z - rhs.z) < DBL_EPSILON;
	}

	bool operator!=(const Point& lhs, const Point& rhs)
	{
		return !(lhs == rhs);
	}

	size_t hash_value(const Point& obj)
	{
		size_t seed = 0x0F09109E;
		seed ^= (seed << 6) + (seed >> 2) + 0x25D877D8 + static_cast<size_t>(obj.x);
		seed ^= (seed << 6) + (seed >> 2) + 0x3E8AE664 + static_cast<size_t>(obj.y);
		seed ^= (seed << 6) + (seed >> 2) + 0x169EA197 + static_cast<size_t>(obj.z);
		return seed;
	}
}