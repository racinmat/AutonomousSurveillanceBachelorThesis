#pragma once
#include <memory>
#include <string>
#include "Dubins/geom/Point.h"
#include "rapidjson/document.h"

using namespace rapidjson;
using namespace std;

namespace App
{
	class Point
	{
	public:
		Point(double x, double y);
		Point(double x, double y, double z);
		Point(const Point& other);
		virtual ~Point();
		double getX() const;
		void setX(double x);
		double getY() const;
		void setY(double y);
		void changeX(double x);
		void changeY(double y);
		virtual double getZ() const;
		virtual void setZ(const double z);
		void changeZ(double z);
		friend ostream& operator<<(ostream& os, const Point& obj);
		string toString();
		double getDistance(shared_ptr<Point> point);
		double getDistanceSquared(shared_ptr<Point> point);
		friend bool operator==(const Point& lhs, const Point& rhs);
		friend bool operator!=(const Point& lhs, const Point& rhs);
		friend size_t hash_value(const Point& obj);
		void moveBy(shared_ptr<Point> point);
		void moveBy(Point point);
		void moveBy(double x, double y, double z);
		geom::Point toGeomPoint();
		Value toJson(Document::AllocatorType& allocator) const;
		static shared_ptr<Point> fromJson(Value data);

	protected:
		double x;
		double y;
		double z;
	};

}
