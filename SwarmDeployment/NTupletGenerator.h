#pragma once
#include <vector>
#include <unordered_map>
#include "Uav.h"

using namespace std;
namespace App
{

	class NTupletGenerator
	{
	public:
		NTupletGenerator();
		virtual ~NTupletGenerator();
		vector<unordered_map<App::Uav, shared_ptr<Point>, UavHasher>> generateNTuplet(vector<shared_ptr<Point>> usedChars, vector<shared_ptr<Uav>> tupletKeys);

	protected:
		vector<unordered_map<App::Uav, shared_ptr<Point>, UavHasher>> generateNTuplet(vector<shared_ptr<Point>> usedChars, vector<shared_ptr<Uav>> tupletKeys, int index);
	};

}
