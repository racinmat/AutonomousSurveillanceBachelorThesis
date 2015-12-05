/*
 * Line.cpp
 *
 *  Created on: 23. 7. 2014
 *      Author: Petr
 */

#include "Line.h"

namespace geom{

Line::Line(const Point & np1, const Point & np2) {
	p1 = np1;
	p2 = np2;
}

Line::Line(Position pos, myFloat len){
	p1 = pos.point;
	p2 = pos.point + pos.getDirection() * len;
}

Position Line::getPosition(myFloat len) const {
	Vector dir = getDirection();
	Point p = p1 + dir.normalize() * len;
	return Position(p, dir.getAngle());
}

bool Line::intersect(Line line) const {
	// normales to directions
	Vector norm1 = this->getDirection().left().normalize();
	Vector norm2 = line.getDirection().left().normalize();

	// end point distances from line1
	myFloat d11 = (line.p1 - this->p1).dotProduct(norm1);
	myFloat d12 = (line.p2 - this->p1).dotProduct(norm1);

	// end point distances from line2
	myFloat d21 = (this->p1 - line.p1).dotProduct(norm2);
	myFloat d22 = (this->p2 - line.p1).dotProduct(norm2);

	// end points of first line are at opposite sides of second line
	if(d11 * d12 < 0 && d21 * d22 < 0){
		return true;
	}

	return false;
}

Intersection Line::intersectionPoint(Line line) const {

	if (intersect(line)) {
		Vector dir2 = line.getDirection();
		Vector normal2 = dir2.normalize().left();

		myFloat dist = normal2.dotProduct(p1 - line.p1);
		myFloat speed = -normal2.dotProduct(getDirection().normalize());

		if (dist * speed > 0) {
			dist = dist / speed;

			Point inter = p1 + getDirection().normalize() * dist;
			return Intersection(Position(inter, getDirection().getAngle()), dist);
			// TODO
			//return Intersection(Position(Point(0,0), 0), 0);
		}
	}

	return Intersection(Position(), 0);
}

} // namespace geom

