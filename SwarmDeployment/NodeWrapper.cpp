#include "NodeWrapper.h"


namespace AStar
{

	NodeWrapper::NodeWrapper()
	{
	}


	NodeWrapper::~NodeWrapper()
	{
	}

	bool NodeWrapper::operator<(const NodeWrapper& another)
	{
		double someBigNumber = 10000;//10000 is arbitrary, I need somehow to compare coordinates by < and > operators. I assume, that map is smaller than 10000 x 10000. 
									 //If map gets bigger, feel free to change this number.
		return  getX() * someBigNumber + getY() < another.getX() * someBigNumber + another.getY(); // keep the same order
	}

	bool NodeWrapper::operator>(const NodeWrapper& another)
	{
		double someBigNumber = 10000;
		return  getX() * someBigNumber + getY() > another.getX() * someBigNumber + another.getY(); // keep the same order
	}

	bool NodeWrapper::operator==(const NodeWrapper& another)
	{
		return  getX() == another.getX() && getY() == another.getY();
	}

	bool NodeWrapper::operator!=(const NodeWrapper& another)
	{
		return !(*this == another);
	}
}