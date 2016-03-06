#include "Rectangle2D.h"
#include "Point2D.h"
#include <algorithm>
#include <Dubins/geom/MyMath.h>

using namespace std;

Rectangle2D::Rectangle2D(double x, double y, double width, double height) : 
	p1(x, y), p2(x + width, y), p3(x, y + height), p4(x + width, y + height)
{

}

Rectangle2D::Rectangle2D(Point2D p1, Point2D p2, Point2D p3, Point2D p4) : p1(p1), p2(p2), p3(p3), p4(p4)
{
}

Rectangle2D::~Rectangle2D()
{
}

void Rectangle2D::rotate(double rotation)
{
	cout << "rotating: " << rotation << endl;

	double x = (p1.getX() + p2.getX() + p3.getX() + p4.getX()) / 4;
	double y = (p1.getY() + p2.getY() + p3.getY() + p4.getY()) / 4;
	Point2D middle(x, y);

	double distance = middle.getDistance(p1);	//distance of corner from middle is same from all corners

	cout << "middle: " << middle.getX() << ", " << middle.getY() << endl;
	cout << "distance: " << distance << endl;

	double angle1 = atan2(p1.getY() - middle.getY(), p1.getX() - middle.getX());
	double angle2 = atan2(p2.getY() - middle.getY(), p2.getX() - middle.getX());
	double angle3 = atan2(p3.getY() - middle.getY(), p3.getX() - middle.getX());
	double angle4 = atan2(p4.getY() - middle.getY(), p4.getX() - middle.getX());

	p1 = Point2D(x + distance * cos(rotation + angle1), y + distance * sin(rotation + angle1));	//rotation of rectangle
	p2 = Point2D(x + distance * cos(rotation + angle2), y + distance * sin(rotation + angle2));
	p3 = Point2D(x + distance * cos(rotation + angle3), y + distance * sin(rotation + angle3));
	p4 = Point2D(x + distance * cos(rotation + angle4), y + distance * sin(rotation + angle4));
}

std::vector<Triangle3D> Rectangle2D::toTriangles()
{
	auto triangles = std::vector<Triangle3D>(2);
	triangles[0] = Triangle3D(p1, p2, p3);
	triangles[1] = Triangle3D(p2, p3, p4);
	return triangles;
}
