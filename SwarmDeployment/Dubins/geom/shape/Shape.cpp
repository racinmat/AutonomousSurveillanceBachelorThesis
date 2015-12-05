/*
 * Shape.cpp
 *
 *  Created on: 20. 7. 2014
 *      Author: Petr
 */

#include "Shape.h"

namespace geom{

Shape::Shape() {
}

Shape::~Shape() {
}

// do nothing ... just becouse of Web-IDL compatibility
bool Shape::pointInPolygon(Point){return false;}
bool Shape::intersect(Line line){return false;}
Point Shape::getCenter(){return Point();}
Point Shape::getEdgePoint(myFloat alpha){return Point();}
myFloat Shape::perimeterLength(){return 0.0;}
int Shape::getType(){return -1;}

// TODO - rewrite for intersected convex regions
myFloat Shape::distanceIfConvex(Shape & other, myFloat & alpha1, myFloat & alpha2){
	myFloat delta1 = 0.1;
	myFloat delta2 = 0.1;

	myFloat dist = getEdgePoint(alpha1).distance(other.getEdgePoint(alpha2));

	while(fabs(delta1) > TOLERANCE || fabs(delta2) > TOLERANCE){
		// try to move with the first point
		myFloat newDistance = getEdgePoint(alpha1 + delta1)
				.distance(other.getEdgePoint(alpha2));
		if(newDistance < dist){
			dist = newDistance;
			alpha1 += delta1;
			delta1 *= 2;
			continue;
		}

		// try to move with the second point
		newDistance = getEdgePoint(alpha1)
				.distance(other.getEdgePoint(alpha2 + delta2));
		if (newDistance < dist) {
			dist = newDistance;
			alpha2 += delta2;
			delta2 *= 2;
			continue;
		}

		delta1 = -sgn(delta1) * fmax(fabs(delta1), TOLERANCE);
		delta2 = -sgn(delta2) * fmax(fabs(delta2), TOLERANCE);

		delta1 /= 2;
		delta2 /= 2;
	}

	return dist;
}

} // namesapce geom
