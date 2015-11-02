#include "Rectangle2D.h"
#include "Point2D.h"


Rectangle2D::Rectangle2D(double x, double y, double width, double height) : 
	x(x), y(y), width(width), height(height)
{

}


Rectangle2D::~Rectangle2D()
{
}

std::vector<Triangle3D*> Rectangle2D::toTriangles()
{
	auto triangles = std::vector<Triangle3D*>(2);
	triangles[0] = new Triangle3D(new Point2D(x, y), new Point2D(x + width, y), new Point2D(x, y + height));
	triangles[1] = new Triangle3D(new Point2D(x + width, y), new Point2D(x, y + height), new Point2D(x + width, y + height));
	return triangles;
}
