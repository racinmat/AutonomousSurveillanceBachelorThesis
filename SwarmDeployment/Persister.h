#pragma once
#include "State.h"

using namespace std;

namespace App
{
	class Configuration;

	class Persister
	{
	public:
		Persister(shared_ptr<Configuration> configuration);
		virtual ~Persister();
		void savePath(vector<shared_ptr<State>> path);
		void savePathToJson(vector<shared_ptr<State>> path, shared_ptr<Map> map);
		void savePathToJson(vector<shared_ptr<State>> path, shared_ptr<Map> map, string postfix);
		void savePathToCsv(vector<shared_ptr<State>> path, string postfix);
		tuple<vector<shared_ptr<State>>, shared_ptr<Map>> loadPathFromJson(string name);
	private:
		void savePathToJsonFile(vector<shared_ptr<State>> path, shared_ptr<Map> map, string name);
		shared_ptr<Configuration> configuration;
	};

}