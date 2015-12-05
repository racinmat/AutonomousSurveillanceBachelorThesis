/*
 * Elipse.h
 *
 *  Created on: 24. 2. 2015
 *      Author: Petr Vana
 */

#pragma once

#include "Shape.h"

namespace geom {

class Ellipse: public Shape {
public:
	Ellipse(Point center, myFloat alpha, myFloat a, myFloat b);
	virtual ~Ellipse();

	Point center;

	// angle of main axis
	myFloat alpha;

	// length of the first axis
	myFloat a;

	// length of the second axis
	myFloat b;

	bool pointInPolygon(Point);

	bool intersect(Line line);

	Point getCenter();

	Point getEdgePoint(myFloat alpha);

	myFloat perimeterLength();

	int getType();
};

} /* namespace geom */
