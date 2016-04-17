#include "Persister.h"
#include <fstream>
#include <json_spirit_v4.08/json_spirit/json_spirit_reader.h>
#include <json_spirit_v4.08/json_spirit/json_spirit_writer.h>
#include "Strings.h"
#include "Configuration.h"
#include <Windows.h>

namespace App
{

	Persister::Persister(shared_ptr<Configuration> configuration) : configuration(configuration)
	{
		HMODULE hModule = GetModuleHandleW(NULL);
		WCHAR wcharPath[MAX_PATH];
		GetModuleFileNameW(hModule, wcharPath, MAX_PATH);
		wstring ws(wcharPath);

		std::string path(ws.begin(), ws.end());
		std::string exeName = "SwarmDeployment.exe";
		path = path.substr(0, path.size() - exeName.size());


		outputDirectory = path + "output/";
	}


	Persister::~Persister()
	{
	}

	void Persister::savePath(vector<shared_ptr<State>> path)
	{
		ofstream file = ofstream(outputDirectory + "path-" + Strings::currentDateTime() +  ".txt");
		for (auto state : path) {
			file << *state << ", " << endl;
		}
		cout << endl;
		file.close();
	}

	void Persister::savePathToJson(vector<shared_ptr<State>> path, shared_ptr<Map> map)
	{
		const string file_name(outputDirectory + "path-" + Strings::currentDateTime() + ".json");
		savePathToJsonFile(path, map, file_name);
	}

	void Persister::savePathToJson(vector<shared_ptr<State>> path, shared_ptr<Map> map, string postfix)
	{
		const string file_name(outputDirectory + "path-" + Strings::currentDateTime() + "-" + postfix + ".json");
		savePathToJsonFile(path, map, file_name);
	}

	void Persister::savePathToCsv(vector<shared_ptr<State>> path, string postfix)
	{
		const string file_name(outputDirectory + "path-" + Strings::currentDateTime() + "-" + postfix + ".csv");
		ofstream os(file_name);         //Opening file to print info to
		for (auto state : path)
		{
			bool firstInLine = true;
			for (auto uav : state->getBaseUavs())
			{
				if (!firstInLine)
				{
					os << "; ";
				}
				auto loc = uav->getPointParticle()->getLocation();
				os << loc->getX() << "; " << loc->getY();
				firstInLine = false;
			}
			os << endl;
		}
		os.close();
	}

	tuple<vector<shared_ptr<State>>, shared_ptr<Map>> Persister::loadPathFromJson(string name)
	{
		ifstream is(name);
		mValue value;
		read(is, value);
		is.close();
		auto mapData = value.get_obj().at("map");
		auto map = Map::fromJson(mapData);
		auto pathData = value.get_obj().at("path");
		auto path = vector<shared_ptr<State>>();
		for (auto stateData : pathData.get_array())
		{
			path.push_back(State::fromJson(stateData));
		}
		return tuple<vector<shared_ptr<State>>, shared_ptr<Map>>(path, map);
	}

	void Persister::writeGraphData(std::map<double, double> data, string name)
	{
		ofstream os(outputDirectory + name + ".csv");
		for (auto &entry : data) {
			os << entry.first << "; " << entry.second << endl;
		}
		os.close();
	}

	void Persister::savePathToJsonFile(vector<shared_ptr<State>> path, shared_ptr<Map> map, string file_name)
	{

		mObject content;

		auto jsonMap  = map->toJson();
		jsonMap["size"] = max(configuration->getWorldHeight(), configuration->getWorldWidth());
		content["map"] = jsonMap;

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
