#include "NTupletGenerator.h"

namespace App
{


	NTupletGenerator::NTupletGenerator()
	{
	}

	NTupletGenerator::~NTupletGenerator()
	{
	}

	vector<unordered_map<App::Uav, shared_ptr<Point>, UavHasher>> NTupletGenerator::generateNTuplet(vector<shared_ptr<Point>> usedChars, vector<shared_ptr<Uav>> tupletKeys)
	{
		return generateNTuplet(usedChars, tupletKeys, tupletKeys.size() - 1);	//index posledn�ho prvku
	}

	vector<unordered_map<Uav, shared_ptr<Point>, UavHasher>> NTupletGenerator::generateNTuplet(vector<shared_ptr<Point>> usedChars, vector<shared_ptr<Uav>> tupletKeys, int index)
	{

		auto list = vector<unordered_map<Uav, shared_ptr<Point>, UavHasher>>();	//todo: pop�em��let, jak to refactorovat, aby se mohlo pracovat s fixn� velikost�  pole
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

		return list;
	}

}
