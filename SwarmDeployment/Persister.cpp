#include "Persister.h"
#include <fstream>
#include "Strings.h"
#include "Configuration.h"
#include <cfloat>
#include <rapidjson/ostreamwrapper.h>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "FileSystem.h"
#include <rapidjson/istreamwrapper.h>

using namespace rapidjson;

namespace App
{

	Persister::Persister(shared_ptr<Configuration> configuration, shared_ptr<DistanceResolver> distanceResolver) : configuration(configuration), distanceResolver(distanceResolver)
	{
		std::string path = FileSystem::getCurrentWorkingDir();
		outputDirectory = path + "output/";

	}


	Persister::~Persister()
	{
	}

	void Persister::savePath(vector<shared_ptr<State>> path)
	{
		bool debug = configuration->getDebug();
		if (debug) {
			cout << "saving foubnd path to directory: " + outputDirectory << endl;
		}


		ofstream file(outputDirectory + "path-" + Strings::currentDateTime() +  ".txt");
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
        IStreamWrapper isw(is);
        Document d;
        d.ParseStream(isw);

		is.close();
		auto &mapData = d["map"];
		auto map = Map::fromJson(mapData);
		auto &pathData = d["path"];
		auto path = vector<shared_ptr<State>>();
		for (auto &stateData : pathData.GetArray())
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

		Document d;
		d.SetObject();
		Document::AllocatorType& allocator = d.GetAllocator();

		auto jsonMap = map->toJson(d);
		jsonMap["size"] = max(configuration->getWorldHeight(), configuration->getWorldWidth());
		d.AddMember("map", jsonMap, allocator);

		Value jsonPath(kArrayType);
		for (auto state : path) {
			jsonPath.PushBack(state->toJson(d), allocator);
		}
		d.AddMember("path", jsonPath, allocator);

		ofstream os(file_name);
        OStreamWrapper osw(os);
        PrettyWriter<OStreamWrapper> writer(osw);
        d.Accept(writer);
		writer.Flush();

		os.close();
	}


}
