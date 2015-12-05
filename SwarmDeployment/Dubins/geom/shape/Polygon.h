/*
 * Polygon.h
 *
 *  Created on: 18. 7. 2014
 *      Author: Petr
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include<vector>

#include "../Point.h"
#include "../Line.h"
#include "../Arc.h"

#include "Shape.h"

using namespace std;

namespace geom{

class Polygon: public Shape {

public:

	vector<Point> pnts;
	Point center;
	myFloat maxRadius;

	Polygon(){};
	Polygon(vector<Point>);
	Polygon(Point center, int n, myFloat radius, myFloat alpha);
	virtual ~Polygon();

	bool pointInPolygon(Point);

	bool intersect(const Line & line) const;
	bool intersectBool(const Arc & arc) const;

	Point getCenter();

private:
	void updateCenter();

public:

	/**
	 *  get Point on edge of this polygon
	 *
	 *  alpha - in interval < 0, 1 >
	 */
	Point getEdgePoint(myFloat alpha);

	myFloat perimeterLength();

	int getType();
};

} // namespace geom

#endif /* POLYGON_H_ */
