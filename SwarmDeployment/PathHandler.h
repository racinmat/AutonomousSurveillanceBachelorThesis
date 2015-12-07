#pragma once
#include <vector>
#include <memory>
#include "State.h"

using namespace std;

namespace App
{

	class PathHandler
	{
	public:
		PathHandler();
		virtual ~PathHandler();
		static vector<shared_ptr<State>> getPath(shared_ptr<State> end);
		static vector<shared_ptr<State>> getPath(shared_ptr<State> start, shared_ptr<State> end);
	};

}