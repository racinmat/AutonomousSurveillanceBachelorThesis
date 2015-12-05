/*
 * Polygon.cpp
 *
 *  Created on: 18. 7. 2014
 *      Author: Petr
 */

#include "Polygon.h"

namespace geom{

Polygon::Polygon(vector<Point> vector) {
	pnts.assign(vector.begin(), vector.end());
	updateCenter();
}

Polygon::Polygon(Point center, int n, myFloat radius, myFloat alpha){
	for (int i = 0; i < n; ++i) {
		Vector dir(((myFloat)i) * 2 * M_PI / n + alpha);
		pnts.push_back(center + radius * dir);
	}
	updateCenter();
}

Polygon::~Polygon() {

}

int Polygon::getType(){
	return 1;
}

bool Polygon::pointInPolygon(Point p) {
	// todo test
	/*if((p-center).lengthSquared() > maxRadius*maxRadius){
		return false;
	}*/

	int i, j;
	int nvert = pnts.size();
	bool c = false;

	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (
				(((pnts[i].y) >= p.y) != (pnts[j].y >= p.y))
				&&
				(p.x <= (pnts[j].x - pnts[i].x) * (p.y - pnts[i].y) / (pnts[j].y - pnts[i].y) + pnts[i].x))
		{
			c = !c;
		}
	}

	return c;
}

bool Polygon::intersect(const Line & line) const {
	// todo test
	/*myFloat rad = line.getLength() + maxRadius;
	if((center - line.p1).lengthSquared() > rad*rad){
		return false;
	}*/

	int size = pnts.size();
	for (int i = 0; i < size; ++i) {
		const Point & p1 = pnts[i];
		const Point & p2 = pnts[(i+1) % size];
		if(line.intersect(Line(p1, p2))){
			return true;
		}
	}
	return false;
}

Point Polygon::getCenter(){
	return center;
}

void Polygon::updateCenter(){
	center = Point();
	for(auto & p : pnts){
		center += p;
	}
	center = center / (myFloat)pnts.size();

	maxRadius = 0;
	for (auto & p : pnts) {
		maxRadius = max (maxRadius, (center - p).length());
	}
}

Point Polygon::getEdgePoint(myFloat alpha){
	// todo - change the way of normalization
	while(alpha > 1){
		alpha --;
	}
	while(alpha < 0){
		alpha ++;
	}

	myFloat requiredLength = perimeterLength() * alpha;
	myFloat length = 0;
	Point* last = & pnts.back();
	for (Point & p : pnts) {
		//if (last) {
		myFloat start = length;
		length += ((*last) - p).length();

		if (requiredLength <= start) {
			return *last;
		}
		if (requiredLength <= length) {
			// interpolate
			myFloat alp = (requiredLength - start) / (length - start);

			//cout << "interpolate" << endl;
			return last->interpolate(p, alp);
		}
		//}
		last = &p;
	}
	//cout << "last" << endl;
	return pnts.back();
}

myFloat Polygon::perimeterLength(){
	myFloat length = 0;
	Point* last = & pnts.back();
	for (Point & p : pnts) {
		length += ((*last) - p).length();
		last = &p;
	}
	return length;
}

bool Polygon::intersectBool(const Arc& arc) const {
	myFloat rad = arc.getLength() / 2 + maxRadius;
	Point arcCenter = (arc.position.point + arc.getEnd().point) / 2;
	if ((arcCenter - center).length() > rad) {
		return false;
	}

	Point last = pnts.back();
	for (Point act : pnts) {
		if (!arc.intersectionPoint(Line(last, act)).position.invalid()) {
			return true;
		}
		last = act;
	}

	return false;
}

} // namespace geom

