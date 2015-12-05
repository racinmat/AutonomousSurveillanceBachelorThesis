/*
 * Point.h
 *
 *  Created on: 18. 7. 2014
 *      Author: Petr
 */

#pragma once

#include<vector>

#include "../Point.h"
#include "../Line.h"

#include "Shape.h"

using namespace std;

namespace geom{

class ShapePoint: public Shape {

public:

	Point point;

	ShapePoint(vector<Point>);
	ShapePoint(Point);
	virtual ~ShapePoint();

	bool intersect(Line line);

	Point getCenter();

	Point getEdgePoint(myFloat alpha);

	myFloat perimeterLength();

	int getType();

	bool pointInPolygon(Point);
};

} // namespace geom

