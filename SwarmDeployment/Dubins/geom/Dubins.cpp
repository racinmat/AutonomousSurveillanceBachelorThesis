/*
 * Dubins.cppy
 *
 *  Created on: 20. 7. 2014
 *      Author: Petr
 */

#include "Dubins.h"
#include <iostream>
#include <limits>

namespace geom
{

int Dubins::initCount = 0;
int Dubins::errorCount = 0;

void Dubins::set(Dubins & from) {
	isCCC = from.isCCC;
	len1 = from.len1;
	len2 = from.len2;
	len3 = from.len3;
	length = from.length;
	radius = from.radius;
}

Dubins::Dubins(Position newStart, Position newEnd, myFloat newRadius) {

	initCount++;

	start = newStart;
	end = newEnd;
	isCCC = false;

	radius = newRadius;

	Vector dir1 = start.getDirection();
	Vector dir2 = end.getDirection();

	Vector dir1radius = dir1 * radius;
	Vector dir2radius = dir2 * radius;

	Point c1left = start.point + dir1radius.left();
	Point c1right = start.point + dir1radius.right();

	Point c2left = end.point + dir2radius.left();
	Point c2right = end.point + dir2radius.right();

	myFloat n1, n2, n3, nLength, centerDistance;
	length = std::numeric_limits<myFloat>::max();

	// RSR - maneuver
	typeOfManeuver = TypeOfManeuver::RSR;

	Vector diff = c2right - c1right;
	myFloat ang = diff.getAngle();

	n1 = angleToRight(start.ang, ang);
	n2 = diff.length();
	n3 = angleToRight(ang, end.ang);

	nLength = n2 + radius * (fabs(n1) + fabs(n3));

	//print(n1, n2, n3, nLength);

	if(nLength < length) {
		len1 = n1;
		len2 = n2;
		len3 = n3;
		length = nLength;
	}

	// LSL - maneuver
	typeOfManeuver = TypeOfManeuver::LSL;

	diff = c2left - c1left;
	ang = diff.getAngle();

	n1 = angleToLeft(start.ang, ang);
	n2 = diff.length();
	n3 = angleToLeft(ang, end.ang);

	nLength = n2 + radius * (fabs(n1) + fabs(n3));

	//print(n1, n2, n3, nLength);

	if (nLength < length) {
		len1 = n1;
		len2 = n2;
		len3 = n3;
		length = nLength;
	}

	// LSR - maneuver

	diff = c2right - c1left;
	centerDistance = diff.length();

	if (centerDistance > 2 * radius) {
		typeOfManeuver = TypeOfManeuver::LSR;

		myFloat alpha = asin(2 * radius / centerDistance);
		myFloat centerAngle = atan2(c2right.y - c1left.y, c2right.x - c1left.x) + alpha;
		n2 = sqrt(centerDistance*centerDistance - 4 * radius * radius);

		// normalize angle
		n1 = angleToLeft(start.ang, centerAngle);
		n3 = angleToRight(centerAngle, end.ang);

		nLength = n2 + radius * (fabs(n1) + fabs(n3));

		//print(n1, n2, n3, nLength);

		if (nLength < length) {
			len1 = n1;
			len2 = n2;
			len3 = n3;
			length = nLength;
		}
	}

	// RSL - maneuver

	diff = c2left - c1right;
	centerDistance = diff.length();

	if (centerDistance > 2 * radius) {
		typeOfManeuver = TypeOfManeuver::RSL;

		myFloat alpha = asin(2 * radius / centerDistance);
		myFloat centerAngle = atan2(c2left.y - c1right.y, c2left.x - c1right.x) - alpha;
		n2 = sqrt(centerDistance * centerDistance - 4 * radius * radius);

		// normalize angle
		n1 = angleToRight(start.ang, centerAngle);
		n3 = angleToLeft(centerAngle, end.ang);

		nLength = n2 + radius * (fabs(n1) + fabs(n3));

		//print(n1, n2, n3, nLength);

		if (nLength < length) {
			len1 = n1;
			len2 = n2;
			len3 = n3;
			length = nLength;
		}
	}

	// CCC maneuver is possible only in case start and end position is close enougth
	if ((start.point - end.point).length() <= 4 * radius) {

		// RLR - maneuver
		diff = c2right - c1right;
		centerDistance = diff.length();

		if (centerDistance < 4 * radius) {
			typeOfManeuver = TypeOfManeuver::RLR;

			// direction of Vector(S1,S2) to Vector(S1,S3)
			myFloat alpha = acos(centerDistance / radius / 4);

			// direction between first and second arc
			myFloat dir12 = diff.getAngle() - M_PI / 2 - alpha;
			// direction between second and third arc
			myFloat dir23 = diff.getAngle() + M_PI / 2 + alpha;

			n1 = angleToRight(start.ang, dir12);
			n2 = angleToLeft(dir12, dir23);
			n3 = angleToRight(dir23, end.ang);

			nLength = radius * (fabs(n1) + fabs(n2) + fabs(n3));

			if (nLength < length) {
				isCCC = true;
				//print(n1, n2, n3, nLength);
				len1 = n1;
				len2 = n2;
				len3 = n3;
				length = nLength;
			}
		}

		// LRL - maneuver
		diff = c2left - c1left;
		centerDistance = diff.length();

		if (centerDistance < 4 * radius) {
			typeOfManeuver = TypeOfManeuver::LRL;

			// direction of Vector(S1,S2) to Vector(S1,S3)
			myFloat alpha = acos(centerDistance / radius / 4);

			// direction between first and second arc
			myFloat dir12 = diff.getAngle() + M_PI / 2 + alpha;
			// direction between second and third arc
			myFloat dir23 = diff.getAngle() - M_PI / 2 - alpha;

			n1 = angleToLeft(start.ang, dir12);
			n2 = angleToRight(dir12, dir23);
			n3 = angleToLeft(dir23, end.ang);

			nLength = radius * (fabs(n1) + fabs(n2) + fabs(n3));

			if (nLength < length) {
				isCCC = true;
				//print(n1, n2, n3, nLength);
				len1 = n1;
				len2 = n2;
				len3 = n3;
				length = nLength;
			}
		}
	}

	/*if (end.point.distance(getSecondArc().getEnd().point) > 1) {
		cout << "dubins -- error (" <<  ++errorCount << "/" << initCount <<  ") ";
		cout << " distance="
				<< end.point.distance(getSecondArc().getEnd().point);
		cout << endl;
		auto & a = *this;
		cout << "isCCC=" << a.isCCC << " ";
		cout << "first=" << a.len1 << " ";
		cout << "second=" << a.len2 << " ";
		cout << "third=" << a.len3 << " ";
		cout << "length=" << a.length << " ";
		cout << "\n";
		cout << endl;
	}*/
}

Dubins::~Dubins() {}

Position Dubins::getPosition(myFloat len) const {
	Arc path1 = getFirstArc();

	if (len < path1.getLength()) {
		return getFirstArc().getPosition(len);
	}

	myFloat l2;
	if (isCCC) {
		Arc ca = getCenterArc();
		l2 = ca.getLength();
		if (len < path1.getLength() + l2) {
			return ca.getPosition(len - path1.getLength());
		}
	} else {
		Line cl = getCenter();
		l2 = cl.getLength();
		if (len < path1.getLength() + l2) {
			return cl.getPosition(len - path1.getLength());
		}
	}

	Arc path3 = getSecondArc();
	return path3.getPosition(len - path1.getLength() - l2);
}

Arc Dubins::getFirstArc() const {
	Position s = start;
	return Arc(s, len1, radius);
}

Line Dubins::getCenter() const {
	return Line(getFirstArc().getEnd(), len2);
}

Arc Dubins::getCenterArc() const {
	Position p = getFirstArc().getEnd();
	return Arc(p, len2, radius);
}

Arc Dubins::getSecondArc() const {
	Position st;
	if (isCCC) {
		st = getCenterArc().getEnd();
	} else {
		Point ps = getCenter().p2;
		st = Position(ps, start.ang + len1);
	}
	return Arc(st, len3, radius);
}

	TypeOfManeuver Dubins::getTypeOfManeuver() const
	{
		return typeOfManeuver;
	}

	void print(myFloat n1, myFloat n2, myFloat n3, myFloat len) {
	std::cout << "\tn1 = " << n1 << "\tn2 = " << n2 << "\tn3 = " << n3
			<< "\tlen = " << len << std::endl;
}

} //namespace geom;
