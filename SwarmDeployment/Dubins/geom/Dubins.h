/*
 * Dubins.h
 *
 *  Created on: 20. 7. 2014
 *      Author: Petr
 */

#pragma once

#include "Line.h"
#include "Path.h"
#include "Arc.h"
#include "Intersection.h"

#include "shape/Shape.h"
#include "shape/Polygon.h"

namespace geom{

	enum class TypeOfManeuver {RSR, LSL, LSR, RSL, RLR, LRL};

class Dubins: public Path {

private:
	// in radians
	myFloat len1, len2, len3;
	TypeOfManeuver typeOfManeuver;

public:

	static int initCount;
	static int errorCount;

	Position start;
	Position end;

	myFloat radius;

	bool isCCC;

	myFloat length;

	Dubins() :
			 len1(NAN), len2(NAN), len3(NAN), radius(NAN), isCCC(false), length(NAN) {
	}

	Dubins(Position, Position, myFloat);

	virtual ~Dubins();

	/*Dubins::Dubins(Dubins & copyFrom){
		set(copyFrom);
	}*/

	void set(Dubins &);
	
	virtual Position getPosition(myFloat len) const override;

	Arc getFirstArc() const;
	Line getCenter() const;
	Arc getCenterArc() const;
	Arc getSecondArc() const;
	TypeOfManeuver getTypeOfManeuver() const;

	Intersection intersectLine(const Line line) const {

		Intersection p = getFirstArc().intersectionPoint(line);
		if (!p.position.invalid()) {
			return p;
		}

		if (isCCC) {
			p = getCenterArc().intersectionPoint(line);
			if (!p.position.invalid()) {
				return p;
			}
		} else {
			p = getCenter().intersectionPoint(line);
			if (!p.position.invalid()) {
				return p;
			}
		}

		p = getSecondArc().intersectionPoint(line);
		if (!p.position.invalid()) {
			return p;
		}

		return Intersection(Position(), 0);
	}

	inline bool intersectLineBool(const Line line) const {
		if (!getFirstArc().intersectionPoint(line).position.invalid()) {
			return true;
		}

		if (isCCC) {
			if (!getCenterArc().intersectionPoint(line).position.invalid()) {
				return true;
			}
		} else {
			if (!getCenter().intersectionPoint(line).position.invalid()) {
				return true;
			}
		}

		if (!getSecondArc().intersectionPoint(line).position.invalid()) {
			return true;
		}

		return false;
	}

	inline Position intersect(const Polygon& polygon) const {
		Intersection in = Intersection(Position(), -1);

		myFloat rad = length/2 + polygon.maxRadius;
		Point dubCenter = (start.point + end.point) / 2;
		if((dubCenter - polygon.center).length() > rad){
			return in.position;
		}

		Point last = polygon.pnts.back();
		for (Point act : polygon.pnts) {
			Intersection in2 = intersectLine(Line(last, act));
			if (!in2.position.invalid()) {
				if (in.distance < 0 || in.distance > in2.distance) {
					in = in2;
				}
			}
			last = act;
		}

		return in.position;
	}

	inline bool intersectBool(const Polygon& polygon) const {
		myFloat rad = length / 2 + polygon.maxRadius;
		Point dubCenter = (start.point + end.point) / 2;
		if ((dubCenter - polygon.center).lengthSquared() > rad*rad) {
			return false;
		}

		// test central part first
		if (isCCC) {
			if (polygon.intersectBool(getCenterArc())) {
				return true;
			}
		} else {
			Line line = getCenter();
			if (polygon.intersect(line)) {
				return true;
			}
		}

		if (polygon.intersectBool(getFirstArc())) {
			return true;
		}

		if (polygon.intersectBool(getSecondArc())) {
			return true;
		}

		return false;
	}

	inline int getCount(){
		return initCount;
	}

	inline myFloat getLength() const {
		return length;
	}
	
	inline bool getIsCCC(){
		return isCCC;
	}

	inline myFloat getLen1() {
		return len1;
	}

	inline myFloat getLen2() {
		return len2;
	}

	inline myFloat getLen3() {
		return len3;
	}

	inline myFloat getRadius() {
		return radius;
	}

	inline myFloat getTotalTurning(){
		return fabs(len1) + fabs(len3);
	}

};

} // namespace geom
