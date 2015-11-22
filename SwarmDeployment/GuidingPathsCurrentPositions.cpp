#include "GuidingPathsCurrentPositions.h"

namespace App {

	GuidingPathsCurrentPositions::GuidingPathsCurrentPositions()
	{
	}
	
	
	GuidingPathsCurrentPositions::~GuidingPathsCurrentPositions()
	{
	}
	
	shared_ptr<Node> GuidingPathsCurrentPositions::get(shared_ptr<Path> path)
	{
		return currentPoint[*path.get()];
	}

	void GuidingPathsCurrentPositions::set(shared_ptr<Path> path, shared_ptr<Node> point)
	{
		currentPoint[*path.get()] = point;
	}
}
