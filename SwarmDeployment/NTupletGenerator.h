#pragma once
#include <vector>
#include <unordered_map>
#include "UavForRRT.h"
#include "CarLikeControl.h"

using namespace std;
namespace App
{

	class NTupletGenerator
	{
	public:
		NTupletGenerator();
		virtual ~NTupletGenerator();
		vector<unordered_map<UavForRRT, CarLikeControl, UavHasher>> generateNTuplet(vector<CarLikeControl> usedChars, vector<shared_ptr<UavForRRT>> tupletKeys);

	protected:
		vector<unordered_map<UavForRRT, CarLikeControl, UavHasher>> generateNTuplet(vector<CarLikeControl> usedChars, vector<shared_ptr<UavForRRT>> tupletKeys, int index);
	};

}
