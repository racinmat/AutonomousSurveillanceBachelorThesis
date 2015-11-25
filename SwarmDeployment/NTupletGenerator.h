#pragma once
#include <vector>
#include <unordered_map>
#include "Uav.h"
#include <map>

using namespace std;
namespace App
{

	class NTupletGenerator
	{
	public:
		NTupletGenerator();
		virtual ~NTupletGenerator();
		vector<unordered_map<App::Uav, shared_ptr<Point>, UavHasher>> generateNTuplet(vector<shared_ptr<Point>> usedChars, vector<shared_ptr<Uav>> tupletKeys, int index);

	protected:
		//this is map<tuple<all input params string representation>, output>. I make string representation from all inputs and compare it to strings in map
		map<string, vector<unordered_map<App::Uav, shared_ptr<Point>, UavHasher>>> cache;	//for same inputs are always same outputs. There will be stored inputs and their outputs.
		bool isInCache(string stringRepresentation);
		string argumentsToString(vector<shared_ptr<Point>> usedChars, vector<shared_ptr<Uav>> tupletKeys, int index);
	};

}
