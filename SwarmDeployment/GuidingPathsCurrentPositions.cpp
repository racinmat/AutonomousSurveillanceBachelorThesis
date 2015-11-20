#include "GuidingPathsCurrentPositions.h"

namespace App {

	GuidingPathsCurrentPositions::GuidingPathsCurrentPositions()
	{
	}
	
	
	GuidingPathsCurrentPositions::~GuidingPathsCurrentPositions()
	{
	}
	
	int GuidingPathsCurrentPositions::get(int index)
	{
		return current_index[index];
	}
	
	void GuidingPathsCurrentPositions::set(int index, int value)
	{
		if (index >= current_index.size())
		{
			current_index.push_back(value);
		} else
		{
			current_index[index] = value;
		}
	}
	
}
