/*
 * MyMath.h
 *
 *  Created on: 20. 7. 2014
 *      Author: Petr
 */

#pragma once

#include <stdlib.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>

namespace geom{

#ifndef M_PI
	#define M_PI 3.141592653589793238462643383279502884 /* pi use l for long double */
#endif

typedef double myFloat;

const myFloat TOLERANCE = 1e-5;

inline myFloat myRandom(){
	return rand() / (myFloat)RAND_MAX;
}

inline int myIntRandom(int size){
	return rand() % size;
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

inline myFloat angleToLeft(const myFloat& ang1, const myFloat& ang2) {
	myFloat ret = ang2 - ang1;

	while (ret > 2 * M_PI - TOLERANCE) {
		ret -= 2 * M_PI;
	}
	while (ret < -TOLERANCE) {
		ret += 2 * M_PI;
	}
	return ret;
}

inline myFloat angleToRight(const myFloat& ang1, const myFloat& ang2) {
	myFloat ret = ang2 - ang1;

	while (ret > TOLERANCE) {
		ret -= 2 * M_PI;
	}
	while (ret < -2 * M_PI + TOLERANCE) {
		ret += 2 * M_PI;
	}

	return ret;
}

inline void randomOrder(std::vector<int> & order, int size) {
	order.clear();
	for(int i = 0; i < size; i++)
		order.push_back(i);
	std::random_shuffle(order.begin(), order.end(), myIntRandom);
}

inline int modulo(int a, int b){
	return (a + b) % b;
}

inline double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

} // namespace geom
