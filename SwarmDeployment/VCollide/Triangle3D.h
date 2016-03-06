#pragma once
#include "Point3D.h"
#include <vector>

class Triangle3D
{
public:
	Triangle3D();
	Triangle3D(Point3D point1, Point3D point2, Point3D point3);
	virtual ~Triangle3D();
	std::vector<double> toVector() const;
	virtual Point3D getPoint1() const;
	virtual Point3D getPoint2() const;
	virtual Point3D getPoint3() const;

protected:
	Point3D point1;
	Point3D point2;
	Point3D point3;

};

