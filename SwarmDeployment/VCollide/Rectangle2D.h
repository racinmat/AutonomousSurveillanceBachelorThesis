#pragma once
#include <vector>
#include "Triangle3D.h"

class Rectangle2D
{
public:
	Rectangle2D(double x, double y, double width, double height);
	virtual ~Rectangle2D();
	std::vector<Triangle3D*> toTriangles();

protected:
	double x;
	double y;
	double width;
	double height;
};
