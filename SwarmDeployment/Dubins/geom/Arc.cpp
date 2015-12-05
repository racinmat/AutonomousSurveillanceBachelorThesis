/*
 * Arc.cpp
 *
 *  Created on: 23. 7. 2014
 *      Author: Petr Vana
 */

#include "Arc.h"
#include <iostream>

namespace geom{

Position Arc::getPosition(myFloat len) const{
	//std::cout << "Arc - angle " << angle << " to " << (len * sgn(angle) / radius) << std::endl;
	return getPositionByAngle(len * sgn(angle) / radius);
	//return Position(getCenter(),0);
}

Intersection Arc::intersectionPoint(const Line & line) const {
	myFloat rad = getLength() + line.getLength();
	if((position.point - line.p1).lengthSquared() > rad*rad){
		return Intersection(Position(), 0);
	}

	// calculate two points of intersection
	Vector dir = line.getDirection().normalize();
	Vector normal = dir.left();
	myFloat distance = normal.dotProduct(getCenter() - line.p1);

	// vector to closest point on line from center of arc
	Vector vDistance = -distance * normal;

	if(distance > radius){
		return Intersection(Position(), 0);
	}

	myFloat tangentAngle = vDistance.getAngle() + (angle > 0 ? M_PI/2 : -M_PI/2);
	myFloat diffAngle = acos(fabs(distance) / radius);

	myFloat ang1 = tangentAngle + diffAngle;
	myFloat ang2 = tangentAngle - diffAngle;

	if(angle > 0){ // left
		myFloat turn1 = angleToLeft(position.ang, ang1);
		myFloat turn2 = angleToLeft(position.ang, ang2);

		myFloat less = std::min(turn1, turn2);
		myFloat more = std::max(turn1, turn2);

		if(less <= angle){
			Position p = getPositionByAngle(less);
			myFloat dist = dir.dotProduct(p.point - line.p1);

			if(dist >= 0 && dist < line.getDirection().length()){
				return Intersection(p, less * radius);
			}

			if (more <= angle) {
				p = getPositionByAngle(more);
				dist = dir.dotProduct(p.point - line.p1);

				if (dist >= 0 && dist < line.getDirection().length()) {
					return Intersection(p, more * radius);
				}
			}
		}
	} else {
		myFloat turn1 = angleToRight(position.ang, ang1);
		myFloat turn2 = angleToRight(position.ang, ang2);

		myFloat less = std::max(turn1, turn2);
		myFloat more = std::min(turn1, turn2);

		if (less >= angle) {
			Position p = getPositionByAngle(less);
			myFloat dist = dir.dotProduct(p.point - line.p1);

			if (dist >= 0 && dist < line.getDirection().length()) {
				return Intersection(p, less * -radius);
			}

			if (more >= angle) {
				p = getPositionByAngle(more);
				dist = dir.dotProduct(p.point - line.p1);

				if (dist >= 0 && dist < line.getDirection().length()) {
					return Intersection(p, more * -radius);
				}
			}
		}
	}

	// todo - try to refactor

	return Intersection(Position(), 0);
}

Point Arc::getCenter() const {
	Point center = position.point;

	Vector toCenter = position.getDirection().left();
	toCenter *= radius;
	if(angle < 0){
		toCenter *= -1;
	}

	center += toCenter;

	return center;
}

Position Arc::getPositionByAngle(myFloat arcAngle) const {
	Point center = getCenter();

	Vector dir = position.getDirection() * radius;
	Vector norm = position.point - center;

	myFloat aa = fabs(arcAngle);
		
	center += dir * sin(aa);
	center += norm * cos(aa);
	
	//Point center2 = getCenter();
	//Point start = position.point;
	//center2 += norm;
	
	/*		std::cout << "c1 " << center.x << "," << center.y 
			<< " aa " << aa 
			<< " c2 " << center2.x << "," << center2.y
			<< " start " << start.x << "," << start.y<< std::endl;*/

	return Position(center, position.ang + arcAngle);
}

Position Arc::getEnd() const {
	return getPositionByAngle(angle);
}


} // namespace geom
