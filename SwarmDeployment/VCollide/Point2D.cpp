#include "Point2D.h"
#include <math.h>

Point2D::Point2D(double x, double y) : Point3D(x, y, 0)
{
}

Point2D::~Point2D()
{
}

double Point2D::getDistance(Point2D point)
{
	return sqrt(pow(fabs(getX() - point.getX()), 2)
		+ pow(fabs(getY() - point.getY()), 2));

}