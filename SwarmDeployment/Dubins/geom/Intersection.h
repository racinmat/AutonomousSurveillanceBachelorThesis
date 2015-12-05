/*
 * Intersection.h
 *
 *  Created on: Sep 22, 2014
 *      Author: petr
 */

#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "Position.h"
#include "MyMath.h"

namespace geom {

class Intersection {
public:

	Position position;
	myFloat distance;

	Intersection(Position position, myFloat distance){
		this->position = position;
		this->distance = distance;
	}

};

} /* namespace geom */

#endif /* INTERSECTION_H_ */
