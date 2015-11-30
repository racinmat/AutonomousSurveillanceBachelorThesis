#pragma once
#include <vector>

class Random
{
public:
	Random();
	virtual ~Random();
	static double inRange(double from, double to);
	static double fromZeroToOne();
	template < typename T>
	static T element(std::vector<T> const vector);
};
