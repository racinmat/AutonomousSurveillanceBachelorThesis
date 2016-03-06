#pragma once
#include <vector>
#include "Triangle3D.h"
#include "Point2D.h"

class Rectangle2D
{
public:
	Rectangle2D(double x, double y, double width, double height);
	Rectangle2D(Point2D p1, Point2D p2, Point2D p3, Point2D p4);
	virtual ~Rectangle2D();
	void rotate(double angle);	//angle muse be in radians
	std::vector<Triangle3D> toTriangles();
	Point2D p1;
	Point2D p2;
	Point2D p3;
	Point2D p4;

protected:
};
