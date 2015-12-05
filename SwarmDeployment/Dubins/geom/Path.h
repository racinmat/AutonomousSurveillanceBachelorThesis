/*
 * Path.h
 *
 *  Created on: Aug 1, 2015
 *      Author: petr
 */

#pragma once

#include "Position.h"

namespace geom {

class Path {
public:
	Path();
	virtual ~Path();

public:
	virtual myFloat getLength() const = 0;

	virtual Position getPosition(myFloat len) const = 0;

	virtual Position getStart() const;
	virtual Position getEnd() const;
};

} /* namespace geom */
