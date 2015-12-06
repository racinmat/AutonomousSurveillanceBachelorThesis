/*
 * dubins_test.cpp
 *
 *  Created on: Oct 11, 2015
 *      Author: Petr Vana
 */

#include "geom/geom.h"
#include <iostream>
#include <map>

using namespace geom;

int dubins_test(int argc, char **argv) {

	const int COUNT = 10000000;

	Position p1 = Position(Point(0,0),0);
	Position p2 = Position(Point(0,0),0);

	time_t t1 = clock();

	Dubins d(p1, p2, 100);

	map<TypeOfManeuver, char*> type;
	type[TypeOfManeuver::RSR] = "RSR";
	type[TypeOfManeuver::LSR] = "LSR";
	type[TypeOfManeuver::RSL] = "RSL";
	type[TypeOfManeuver::LRL] = "LRL";
	type[TypeOfManeuver::LSL] = "LSL";
	type[TypeOfManeuver::RLR] = "RLR";

	for (int i = 0; i < COUNT; ++i) {
		p1.random(100);
		p2.random(100);
		d = Dubins(p1, p2, 100);
		std::cout << type[d.getTypeOfManeuver()] << endl;
		std::cout << d.isCCC << endl;
	}

	time_t t2 = clock();
	std::cout << (t2 - t1) * 1000 / CLOCKS_PER_SEC << " ms" << std::endl;

	return 0;
}


