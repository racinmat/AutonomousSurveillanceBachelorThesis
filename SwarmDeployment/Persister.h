#pragma once
#include "State.h"

using namespace std;

namespace App
{

	class Persister
	{
	public:
		Persister();
		virtual ~Persister();
		void savePath(vector<shared_ptr<State>> path);
		void savePathToJson(vector<shared_ptr<State>> path, shared_ptr<Map> map);
		vector<shared_ptr<State>> loadPath();
	};

}