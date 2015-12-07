#pragma once
#include <vector>

using namespace std;

class NumberVector<_T> : public vector<_T>
{
public:
	NumberVector();
	virtual ~NumberVector();
};

