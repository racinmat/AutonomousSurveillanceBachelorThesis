/*
 * Circle.cpp
 *
 *  Created on: 24. 2. 2015
 *      Author: Petr Vana
 */

#include "Circle.h"

namespace geom {

Circle::Circle(Point center, myFloat radius) {
	this->center = center;
	this->radius = radius;
}

Circle::~Circle() {}

bool Circle::pointInPolygon(Point p){
	return center.distanceSquared(p) <= radius * radius;
}

bool Circle::intersect(Line line){
	// TODO
	return false;
}

Point Circle::getCenter(){
	return center;
}

Point Circle::getEdgePoint(myFloat alpha){
	return center + radius * Vector(alpha * 2 * M_PI);
}

myFloat Circle::perimeterLength() {
	return 2 * M_PI * radius;
}
;

int Circle::getType() {
	return 3;
}
;

} /* namespace geom */
