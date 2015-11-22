#pragma once
#include <memory>
#include "Path.h"
#include <unordered_map>
#include "Node.h"

namespace App
{

	class GuidingPathsCurrentPositions
	{
	public:
		GuidingPathsCurrentPositions();
		virtual ~GuidingPathsCurrentPositions();
		virtual shared_ptr<Node> get(shared_ptr<Path> path);
		virtual void set(shared_ptr<Path> path, shared_ptr<Node> point);
	protected:
		unordered_map<Path, shared_ptr<Node>, PathHasher> currentPoint;	//todo: udìlat equals a hasher pro Path
	};

}
