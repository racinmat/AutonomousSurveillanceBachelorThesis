#include "Rectangle2D.h"
#include "Point2D.h"


Rectangle2D::Rectangle2D(double x, double y, double width, double height) : 
	x(x), y(y), width(width), height(height)
{

}


Rectangle2D::~Rectangle2D()
{
}

std::vector<Triangle3D> Rectangle2D::toTriangles()
{
	auto triangles = std::vector<Triangle3D>(2);
	triangles[0] = Triangle3D(Point2D(x, y), Point2D(x + width, y), Point2D(x, y + height));
	triangles[1] = Triangle3D(Point2D(x + width, y), Point2D(x, y + height), Point2D(x + width, y + height));
	return triangles;
}
