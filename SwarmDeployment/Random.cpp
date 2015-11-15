#include "Random.h"
#include <stdlib.h>


Random::Random()
{
}


Random::~Random()
{
}

double Random::inRange(double from, double to)
{
	if (from > to)
	{
		throw "From must not be greater than to.";
	}
	return fromZeroToOne() * (to - from) + from;
}

double Random::fromZeroToOne()
{
//	return (double(rand()) / (RAND_MAX));	//random number from 0 to 1
	return 0.5;	//pro kontrolu vùèi matlabu
}