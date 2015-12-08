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
		static vector<shared_ptr<LinkedState>> getPath(shared_ptr<LinkedState> end);
		static vector<shared_ptr<LinkedState>> getPath(shared_ptr<LinkedState> start, shared_ptr<LinkedState> end);
	};

}