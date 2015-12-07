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
	//když pøesunu tìlo funkce do cpp souboru, pøestane fungovat kompilace, because fuck you, C++. http://stackoverflow.com/questions/3040480/c-template-function-compiles-in-header-but-not-implementation/3040706#3040706
	static T element(std::vector<T> const vector)
	{
		return vector[index(vector)];
	}
	template < typename T>
	static int index(std::vector<T> const vector)
	{
		return rand() % vector.size();
	}
};
