/*
 * Circle.h
 *
 *  Created on: 24. 2. 2015
 *      Author: Petr Vana
 */

#pragma once

#include "../Point.h"
#include "Shape.h"

namespace geom {

class Circle: public Shape {
public:

	Point center;
	myFloat radius;

	inline Circle(): radius(NAN){};
	Circle(Point center, myFloat radius);
	virtual ~Circle();

	bool pointInPolygon(Point);

	bool intersect(Line line);

	Point getCenter();

	Point getEdgePoint(myFloat alpha);

	myFloat perimeterLength();
	
	inline myFloat getRadius(){return radius;}

	int getType();
};

} /* namespace geom */
