#include "Persister.h"
#include <fstream>
#include <json_spirit/json_spirit_reader.h>
#include <json_spirit/json_spirit_writer.h>
#include "Strings.h"
#include "Configuration.h"
#include <Windows.h>
#include <cfloat>

namespace App
{

	Persister::Persister(shared_ptr<Configuration> configuration, shared_ptr<DistanceResolver> distanceResolver) : configuration(configuration), distanceResolver(distanceResolver)
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

	void Persister::writePathData(vector<shared_ptr<State>> path)
	{
		int uavCount = path[0]->getUavs().size();
		vector<std::map<double, double>> distancesToNearestNeighbour(uavCount);

		std::map<double, double> distanceToGoal;
		int iter = 0;


		auto goalState = path[path.size() - 1];
		for (auto state : path)
		{
			iter++;

			for (size_t i = 0; i < state->getBaseUavs().size(); i++)
			{
				auto uav = state->getBaseUavs()[i];
				double distanceToNearestNeighbour = DBL_MAX;
				for (size_t j = 0; j < state->getBaseUavs().size(); j++)
				{
					if (i == j)
					{
						continue;
					}
					auto anotherUav = state->getBaseUavs()[j];
					auto distance = uav->getPointParticle()->getLocation()->getDistance(anotherUav->getPointParticle()->getLocation());
					if (distance < distanceToNearestNeighbour)
					{
						distanceToNearestNeighbour = distance;
					}
				}
				distancesToNearestNeighbour[i][iter] = distanceToNearestNeighbour;
			}

			distanceToGoal[iter] = distanceResolver->getDistance(state, goalState);
		}

		for (size_t i = 0; i < distancesToNearestNeighbour.size(); i++)
		{
			writeGraphData(distancesToNearestNeighbour[i], "distances-to-nearest-neighbour-" + to_string(i) + "-" + Strings::currentDateTime());
		}
		writeGraphData(distanceToGoal, "distance-to-goal-" + Strings::currentDateTime());
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
