#pragma once
#include <vector>
#include <unordered_map>
#include "Uav.h"
#include "CarLikeControl.h"

using namespace std;
namespace App
{

	class NTupletGenerator
	{
	public:
		NTupletGenerator();
		virtual ~NTupletGenerator();
		vector<unordered_map<Uav, shared_ptr<CarLikeControl>, UavHasher>> generateNTuplet(vector<shared_ptr<CarLikeControl>> usedChars, vector<shared_ptr<Uav>> tupletKeys);

	protected:
		vector<unordered_map<Uav, shared_ptr<CarLikeControl>, UavHasher>> generateNTuplet(vector<shared_ptr<CarLikeControl>> usedChars, vector<shared_ptr<Uav>> tupletKeys, int index);
	};

}
