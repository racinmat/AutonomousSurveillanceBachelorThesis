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

	map<TypeOfManeuver, char*> type;
	type[TypeOfManeuver::RSR] = "RSR";
	type[TypeOfManeuver::LSR] = "LSR";
	type[TypeOfManeuver::RSL] = "RSL";
	type[TypeOfManeuver::LRL] = "LRL";
	type[TypeOfManeuver::LSL] = "LSL";
	type[TypeOfManeuver::RLR] = "RLR";

	const int COUNT = 1000;

	time_t t1 = clock();

	Position p1 = Position(Point(0,0),0);
	Position p2 = Position(Point(0,0),0);

	Dubins d(p1, p2, 5);

	std::cout << d.getLen1() << endl;
	std::cout << d.getLen2() << endl;
	std::cout << d.getLen3() << endl;
	std::cout << d.getLength() << endl;
	std::cout << type[d.getTypeOfManeuver()] << endl;

	p1 = Position(Point(0, 0), 0);
	p2 = Position(Point(10, 10), 0);

	d = Dubins(p1, p2, 5);

	std::cout << d.getLen1() << endl;
	std::cout << d.getLen2() << endl;
	std::cout << d.getLen3() << endl;
	std::cout << d.getLength() << endl;
	std::cout << type[d.getTypeOfManeuver()] << endl;

	p1 = Position(Point(0, 0), 0);
	p2 = Position(Point(10, 20), 0);

	d = Dubins(p1, p2, 5);

	std::cout << d.getLen1() << endl;
	std::cout << d.getLen2() << endl;
	std::cout << d.getLen3() << endl;
	std::cout << d.getLength() << endl;
	std::cout << type[d.getTypeOfManeuver()] << endl;

	p1 = Position(Point(0, 0), 0);
	p2 = Position(Point(0, 10), 0);

	d = Dubins(p1, p2, 5);

	std::cout << d.getLen1() << endl;
	std::cout << d.getLen2() << endl;
	std::cout << d.getLen3() << endl;
	std::cout << d.getLength() << endl;
	std::cout << type[d.getTypeOfManeuver()] << endl;

	p1 = Position(Point(0, 0), 0);
	p2 = Position(Point(10, 0), 0);

	d = Dubins(p1, p2, 5);

	std::cout << d.getLen1() << endl;	//len u kružnice je pouze úhlový, v radiánech, nepoèítá s polomìrem. vrací to orientovaný úhel, kladný proti smìru hodinových ruèièek, záporný po smìru hodinových ruèièek
	std::cout << d.getLen2() << endl;
	std::cout << d.getLen3() << endl;
	std::cout << d.getLength() << endl;
	std::cout << type[d.getTypeOfManeuver()] << endl;



//	map<TypeOfManeuver, bool> used;
//	used[TypeOfManeuver::RSR] = false;
//	used[TypeOfManeuver::LSR] = false;
//	used[TypeOfManeuver::RSL] = false;
//	used[TypeOfManeuver::LRL] = false;
//	used[TypeOfManeuver::LSL] = false;
//	used[TypeOfManeuver::RLR] = false;
//
//		
//	for (int i = 0; i < COUNT; ++i) {
//		p1.random(100);
//		p2.random(100);
//		d = Dubins(p1, p2, 100);
//		std::cout << d.getLen1() << endl;
//		std::cout << d.getLen2() << endl;
//		std::cout << d.getLen3() << endl;
//		std::cout << type[d.getTypeOfManeuver()] << endl;
//		used[d.getTypeOfManeuver()] = true;
//	}
//	
//	cout << "end" << endl;
//	for (auto one : used)
//	{
//		cout << one.second << endl;
//	}

	time_t t2 = clock();
	std::cout << (t2 - t1) * 1000 / CLOCKS_PER_SEC << " ms" << std::endl;

	cin.get();

	return 0;
}


