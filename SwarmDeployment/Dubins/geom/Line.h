/*
 * Line.h
 *
 *  Created on: 23. 7. 2014
 *      Author: Petr
 */

#pragma once

#include "Path.h"
#include "Intersection.h"

namespace geom{

class Line: public Path {
public:

	Point p1;
	Point p2;

	inline Line(){};
	Line(const Point &, const Point &);
	Line(Position, myFloat);

	virtual Position getPosition(myFloat len) const;

	inline myFloat getLength() const {
		return (p2 - p1).length();
	}

	inline Vector getDirection() const {
		return p2 - p1;
	}

	bool intersect(Line line) const;
	Intersection intersectionPoint(Line line) const;
	
};

} /* namespace geom */

