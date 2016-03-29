#pragma once
#include "Point.h"
#include "UavForRRT.h"
#include "NTupletGenerator.h"
#include <map>
#include "CarLikeControl.h"

namespace App
{
	
class InputGenerator
{
public:
	InputGenerator(int input_samples_dist, int input_samples_phi);
	virtual ~InputGenerator();
	vector<unordered_map<UavForRRT, CarLikeControl, UavHasher>> generateAllInputs(int distance_of_new_nodes, double max_turn, vector<shared_ptr<UavForRRT>> uavs, bool zeroInputEnabled);

protected:
	NTupletGenerator generator;
	vector<CarLikeControl> generateOneUavInputs(int distance_of_new_nodes, double max_turn, bool zeroInputEnabled);
	int input_samples_dist;	//poèítám s tím, že se bìhem bìhu aplikace nemìní, proto jsou to fieldy a ne argumenty funkcí
	int input_samples_phi;

	//this is map<tuple<all input params string representation>, output>. I make string representation from all inputs and compare it to strings in map
	map<string, vector<unordered_map<UavForRRT, CarLikeControl, UavHasher>>> cache;	//for same inputs are always same outputs. There will be stored inputs and their outputs.
	bool isInCache(string stringRepresentation);
	string argumentsToString(int distance_of_new_nodes, double max_turn, vector<shared_ptr<UavForRRT>> uavs);
};

}
