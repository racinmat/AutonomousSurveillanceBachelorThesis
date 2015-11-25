#include "NTupletGenerator.h"
#include <string>

namespace App
{


	NTupletGenerator::NTupletGenerator()
	{
	}

	NTupletGenerator::~NTupletGenerator()
	{
	}

	vector<unordered_map<Uav, shared_ptr<Point>, UavHasher>> NTupletGenerator::generateNTuplet(vector<shared_ptr<Point>> usedChars, vector<shared_ptr<Uav>> tupletKeys, int index)
	{
//		string stringRepresentation = argumentsToString(usedChars, tupletKeys, index);
//		if (isInCache(stringRepresentation))
//		{
//			return cache[stringRepresentation];
//		}

		auto list = vector<unordered_map<Uav, shared_ptr<Point>, UavHasher>>();	//todo: popøemýšlet, jak to refactorovat, aby se mohlo pracovat s fixní velikostí  pole
																									//		vector<vector<T>> list = vector<vector<T>>(pow(usedChars.size(), tupletClass));
		if (index < 0)
		{
			list.push_back(unordered_map<Uav, shared_ptr<Point>, UavHasher>());
		}
		else
		{
			vector<unordered_map<Uav, shared_ptr<Point>, UavHasher>> tuplet = generateNTuplet(usedChars, tupletKeys, index - 1);
			for (shared_ptr<Point> character : usedChars)
			{
				for (auto row : tuplet)
				{
					row[*tupletKeys[index].get()] = character;
					list.push_back(row);
				}
			}
		}

//		cache[stringRepresentation] = list;
		return list;
	}

	bool NTupletGenerator::isInCache(string stringRepresentation)
	{
		for (auto pair : cache)
		{
			string key = pair.first;
			if (key == stringRepresentation)
			{
				return true;
			}
		}
		return false;
	}

	string NTupletGenerator::argumentsToString(vector<shared_ptr<Point>> usedChars, vector<shared_ptr<Uav>> tupletKeys, int index)
	{
		string string = "";
		for (auto point : usedChars)
		{
			string += "x:" + to_string(point->getX()) + "y:" + to_string(point->getY()) + "z:" + to_string(point->getZ());
		}
		for (auto uav : tupletKeys)
		{
			string += "id:" + to_string(uav->getId());
		}
		string += to_string(index);
		return string;
	}
}
