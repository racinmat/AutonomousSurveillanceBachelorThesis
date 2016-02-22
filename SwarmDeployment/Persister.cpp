#include "Persister.h"
#include <fstream>
#include <json_spirit_v4.08/json_spirit/json_spirit_reader.h>
#include <json_spirit_v4.08/json_spirit/json_spirit_writer.h>
#include "Strings.h"

//include <boost/algorithm/string.hpp>
//include <boost/algorithm/string/split.hpp>

namespace App
{

	Persister::Persister()
	{
	}


	Persister::~Persister()
	{
	}

	void Persister::savePath(vector<shared_ptr<State>> path)
	{
		ofstream file = ofstream("path-" + Strings::currentDateTime() +  ".txt");
		for (auto state : path) {
			file << *state << ", " << endl;
		}
		cout << endl;
		file.close();
	}

	void Persister::savePathToJson(vector<shared_ptr<State>> path, shared_ptr<Map> map)
	{
		const string file_name("path-" + Strings::currentDateTime() + ".json");

		mObject content;

		content["map"] = map->toJson();

		mArray jsonPath;
		for (auto state : path) {
			jsonPath.push_back(state->toJson());
		}
		content["path"] = jsonPath;

		ofstream os(file_name);

		write_formatted(content, os);

		os.close();

	}

	vector<shared_ptr<State>> Persister::loadPath()
	{
//		ifstream file("path.txt");
//		string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
//		vector<string> states;
//		split(states, content, boost::is_any_of(",\nuavs: \n"));

		//todo: implement
		return vector<shared_ptr<State>>();
	}

}
