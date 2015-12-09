#pragma once
#include <vector>
#include <memory>
#include "State.h"
#include "CollisionDetector.h"

using namespace std;

namespace App
{

	class PathHandler
	{
	public:
		PathHandler(shared_ptr<CollisionDetector> collisionDetector);
		virtual ~PathHandler();
		static vector<shared_ptr<LinkedState>> getPath(shared_ptr<LinkedState> end);
		static vector<shared_ptr<State>> createStatePath(vector<shared_ptr<LinkedState>> path);	//rebuilds path with non linked State
		static vector<shared_ptr<LinkedState>> getPath(shared_ptr<LinkedState> start, shared_ptr<LinkedState> end);
		void straightenCrossingTrajectories(vector<shared_ptr<State>> path);

	protected:
		shared_ptr<CollisionDetector> collisionDetector;
	};

}