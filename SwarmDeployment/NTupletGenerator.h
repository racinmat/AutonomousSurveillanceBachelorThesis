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
		vector<unordered_map<Uav, CarLikeControl, UavHasher>> generateNTuplet(vector<CarLikeControl> usedChars, vector<shared_ptr<Uav>> tupletKeys);

	protected:
		vector<unordered_map<Uav, CarLikeControl, UavHasher>> generateNTuplet(vector<CarLikeControl> usedChars, vector<shared_ptr<Uav>> tupletKeys, int index);
	};

}
