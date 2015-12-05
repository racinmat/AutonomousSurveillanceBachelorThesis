/*
 * Elipse.cpp
 *
 *  Created on: 24. 2. 2015
 *      Author: petr
 */

#include "Ellipse.h"

namespace geom{

Ellipse::Ellipse(Point center, myFloat alpha, myFloat a, myFloat b) {
	this->center = center;
	this->alpha = alpha;
	this->a = a;
	this->b = b;
}

Ellipse::~Ellipse() {}

bool Ellipse::pointInPolygon(Point){
	// TODO
	return false;
};

bool Ellipse::intersect(Line line){
	// TODO
	return true;
};

Point Ellipse::getCenter(){
	return center;
};

Point Ellipse::getEdgePoint(myFloat beta){
	auto s = sin(beta * 2 * M_PI);
	auto c = cos(beta * 2 * M_PI);

	Vector va = a * Vector(alpha);
	Vector vb = b * Vector(alpha + M_PI/2);

	return center + c * va + s * vb;
};

myFloat Ellipse::perimeterLength(){
	// TODO - write correct solution !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
	return 2 * M_PI * sqrt(a*a + b*b)/2;
};

int Ellipse::getType(){
	return 4;
};

} /* namespace geom */
