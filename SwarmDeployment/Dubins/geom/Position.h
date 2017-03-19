/*
 * Position.h
 *
 *  Created on: 18. 7. 2014
 *      Author: Petr
 */

#ifndef POSITION_H_
#define POSITION_H_

#include "MyMath.h"
#include "Vector.h"
#include "Point.h"

namespace geom{

class Position final {
public:
	Point point;
	myFloat ang;

	Position();
	Position(Point, myFloat);
	Position(myFloat, myFloat, myFloat);

	inline void set(myFloat x, myFloat y, myFloat ang){
		point.x = x;
		point.y = y;
		this->ang = ang;
	}

	void random(myFloat);
	Vector getDirection() const;

	inline bool invalid(){
		return std::isnan(ang);
	}

	inline Point getPoint(){return point;}
	inline myFloat getAngle(){return ang;}

	inline myFloat squared_distance(const Position& p) const {
		return point.distanceSquared(p.point);
	}

	//TODO - add method witch will check tolerance
	//bool isInTolerance
};

} // namespace geom

#endif /* POSITION_H_ */
