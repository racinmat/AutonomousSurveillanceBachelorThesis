/*
 * Position.cpp
 *
 *  Created on: 18. 7. 2014
 *      Author: Petr
 */

#include "Position.h"

namespace geom{

Position::Position() {
	this->point = Point(0, 0);
	this->ang = NAN;
}

Position::Position(Point point, myFloat angle) {
	this->point = point;
	this->ang = angle;
}

Position::Position(myFloat x, myFloat y, myFloat angle) {
	this->point = Point(x,y);
	this->ang = angle;
}

void Position::random(myFloat interval){
	this->point = Point(myRandom() * interval, myRandom() * interval);
	this->ang = myRandom() * interval;
}

Vector Position::getDirection() const {
	return Vector(cos(ang), sin(ang));
}

} // namespace geom

