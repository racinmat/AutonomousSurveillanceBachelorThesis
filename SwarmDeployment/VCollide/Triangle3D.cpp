#include "Triangle3D.h"
#include "Point2D.h"


Triangle3D::Triangle3D() : 
	point1(Point2D(0, 0)), point2(Point2D(0, 0)), point3(Point2D(0, 0))
{
}

Triangle3D::Triangle3D(Point3D point1, Point3D point2, Point3D point3) :
	point1(point1), point2(point2), point3(point3)
{
}

Triangle3D::~Triangle3D()
{
}

std::vector<double> Triangle3D::toVector() const
{
	return std::vector<double> ({ point1.getX(), point1.getY(), point1.getZ(), point2.getX(), point2.getY(), point2.getZ(), point3.getX(), point3.getY(), point3.getZ() });
}