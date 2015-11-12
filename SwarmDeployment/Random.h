#pragma once
class Random
{
public:
	Random();
	virtual ~Random();
	static double inRange(double from, double to);
	static double fromZeroToOne();
};

