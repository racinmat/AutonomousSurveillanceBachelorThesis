/*
 * Point.cpp
 *
 *  Created on: 18. 7. 2014
 *      Author: Petr
 */

#include "ShapePoint.h"

namespace geom{

ShapePoint::ShapePoint(vector<Point> vector) {
	point = Point();
	for(Point p : vector){
		point += p;
	}
	point = point / vector.size();
}

ShapePoint::ShapePoint(Point p) {
	point = p;
}

ShapePoint::~ShapePoint() {

}

int ShapePoint::getType(){
	return 2;
}

bool ShapePoint::intersect(Line line){
	// no intersect
	return false;
}

Point ShapePoint::getCenter(){
	return point;
}

Point ShapePoint::getEdgePoint(myFloat alpha){
	return point;
}

myFloat ShapePoint::perimeterLength(){
	return 0;
}

bool ShapePoint::pointInPolygon(Point p){
	return false;
}

} // namespace geom

