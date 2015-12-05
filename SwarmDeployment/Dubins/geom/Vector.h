/*
 * Vector.h
 *
 *  Created on: 20. 7. 2014
 *      Author: Petr
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include "MyMath.h"

namespace geom{

class Vector {
public:

	myFloat dx, dy;

	Vector(myFloat x, myFloat y) :
			dx(x), dy(y) {
	}

	Vector(myFloat angle) :
			dx(cos(angle)), dy(sin(angle)) {
	}

	inline myFloat getAngle(){
		return atan2(dy, dx);
	}

	inline Vector right(){
		return Vector(dy, -dx);
	}

	inline Vector left(){
		return Vector(-dy, dx);
	}

	inline Vector negate(){
		return Vector(-dx, -dy);
	}

	inline myFloat length(){
		return sqrt(dx*dx + dy*dy);
	}

	inline myFloat lengthSquared() const {
			return dx*dx + dy*dy;
		}

	inline Vector normalize(){
		myFloat len = this->length();
		return Vector(dx/len, dy/len);
	}

	inline myFloat dotProduct(Vector vec){
		return this->dx * vec.dx + this->dy * vec.dy;
	}

	inline Vector& operator*= (const myFloat& mult ){
		dx *= mult;
		dy *= mult;
		return *this;
	}

	inline Vector& operator+=(const Vector& rght) {
		dx += rght.dx;
		dy += rght.dy;
		return *this;
	}

};

inline Vector operator+ (const Vector& left, const Vector& right){
	return Vector(left.dx + right.dx, left.dy + right.dy);
}

inline Vector operator* (const Vector& left, const myFloat& right){
	return Vector(left.dx * right, left.dy * right);
}

inline Vector operator* (const myFloat& left, const Vector& right){
	return Vector(right.dx * left, right.dy * left);
}

} // namespace geom

#endif /* VECTOR_H_ */
