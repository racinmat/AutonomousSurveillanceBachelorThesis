#pragma once
#include <memory>

namespace App
{
	class Point
	{
	public:
		Point(double x, double y);
		Point(double x, double y, double z);
		Point(const Point& other);
		~Point();
		double getX() const;
		void setX(double x);
		double getY() const;
		void setY(double y);
		void changeX(double x);
		void changeY(double y);
		virtual double getZ() const;
		virtual void setZ(const double z);
		void changeZ(double z);
		friend std::ostream& operator<<(std::ostream& os, const Point& obj);
		double getDistance(std::shared_ptr<Point> point);
		double getDistanceSquared(std::shared_ptr<Point> point);
		friend bool operator==(const Point& lhs, const Point& rhs);
		friend bool operator!=(const Point& lhs, const Point& rhs);
		friend std::size_t hash_value(const Point& obj);

	protected:
		double x;
		double y;
		double z;
	};

}
