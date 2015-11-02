#pragma once
#include "Point3D.h"
#include <vector>

class Triangle3D
{
public:
	Triangle3D(Point3D* point1, Point3D* point2, Point3D* point3);
	virtual ~Triangle3D();
	double* toArray() const;
	std::vector<double> toVector() const;

protected:
	Point3D* point1;
	Point3D* point2;
	Point3D* point3;

};

