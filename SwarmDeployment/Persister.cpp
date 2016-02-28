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
		savePathToJsonFile(path, map, file_name);
	}

	void Persister::savePathToJson(vector<shared_ptr<State>> path, shared_ptr<Map> map, string postfix)
	{
		const string file_name("path-" + Strings::currentDateTime() + "-" + postfix + ".json");
		savePathToJsonFile(path, map, file_name);
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

	void Persister::loadPathFromJson(string name)
	{
		ifstream is(name);
		mValue value;
		read(is, value);
		is.close();
		auto obj = value.get_obj();
		auto map = value.get_obj().at("map");
		auto goals = map.get_obj().at("goals");
		for(auto goal : goals.get_array())
		{
			auto height = goal.get_obj().at("height").get_int();
			auto width = goal.get_obj().at("width").get_int();
			auto location = goal.get_obj().at("location");
			auto x = location.get_obj().at("x").get_int();
			auto y = location.get_obj().at("y").get_int();
		}
		auto obstacles = map.get_obj().at("obstacles");
		for (auto goal : obstacles.get_array())
		{
			auto height = goal.get_obj().at("height").get_int();
			auto width = goal.get_obj().at("width").get_int();
			auto location = goal.get_obj().at("location");
			auto x = location.get_obj().at("x").get_int();
			auto y = location.get_obj().at("y").get_int();
		}
		auto path = value.get_obj().at("path");
		for (auto state : path.get_array())
		{
			for (auto uavData : state.get_obj())
			{
//				auto idString = uavData.first;
				auto uav = uavData.second;
				auto id = uav.get_obj().at("id").get_int();
				auto pointParticle = uav.get_obj().at("pointParticle");
				auto location = pointParticle.get_obj().at("location");
				auto x = location.get_obj().at("x").get_real();
				auto y = location.get_obj().at("y").get_real();
				auto rotation = pointParticle.get_obj().at("rotation");
				auto z = location.get_obj().at("z").get_real();
				auto previousInput = uav.get_obj().at("previousInput");
				auto step = previousInput.get_obj().at("step").get_int();
				auto turn = previousInput.get_obj().at("step").get_real();
			}
		}
	}

	void Persister::savePathToJsonFile(vector<shared_ptr<State>> path, shared_ptr<Map> map, string file_name)
	{

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
}
