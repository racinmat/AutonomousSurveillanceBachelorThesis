/*
 * Arc.h
 *
 *  Created on: 23. 7. 2014
 *      Author: Petr
 */

#pragma once

#include "Path.h"
#include "Line.h"
#include "Intersection.h"

namespace geom{

class Arc: public Path {
public:

	Position position;
	myFloat angle;
	myFloat radius;

	Arc() :
			angle(NAN), radius(NAN) {
	}

	Arc(Position & position, myFloat angle, myFloat radius) :
			position(position), angle(angle), radius(radius) {
	}

	virtual Position getPosition(myFloat len) const;

	Intersection intersectionPoint(const Line & line) const;
	Point getCenter() const;
	Position getPositionByAngle(myFloat angle) const;
	Position getEnd() const;

	//bool intersectBool(const Polygon& polygon) const;

	inline myFloat getLength() const{
		return fabs(getAngle()) * getRadius();
	}

	inline myFloat getAngle() const {return angle;}
	inline myFloat getRadius() const {return radius;}
	inline Position getStart() const {return position;}
};

} // namespace geom
