/*
 * Path.cpp
 *
 *  Created on: Aug 1, 2015
 *      Author: petr
 */

#include "Path.h"

namespace geom {

Path::Path() {}
Path::~Path() {}

Position Path::getStart() const {
	return getPosition(0);
}

Position Path::getEnd() const {
	return getPosition(getLength());
}

} /* namespace geom */
