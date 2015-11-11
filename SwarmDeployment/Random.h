#pragma once
class Random
{
public:
	Random();
	virtual ~Random();
	static double inRange(int from, int to);
	static double fromZeroToOne();
};

