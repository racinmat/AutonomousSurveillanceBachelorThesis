#pragma once
#include "Point.h"
#include "Uav.h"
#include "NTupletGenerator.h"

namespace App
{
	
class InputGenerator
{
public:
	InputGenerator(int input_samples_dist, int input_samples_phi);
	virtual ~InputGenerator();
	vector<unordered_map<Uav, shared_ptr<Point>, UavHasher>> generateAllInputs(int distance_of_new_nodes, double max_turn, vector<shared_ptr<Uav>> uavs);

protected:
	NTupletGenerator generator;
	vector<shared_ptr<Point>> generateOneUavInputs(int distance_of_new_nodes, double max_turn);
	int input_samples_dist;	//po��t�m s t�m, �e se b�hem b�hu aplikace nem�n�, proto jsou to fieldy a ne argumenty funkc�
	int input_samples_phi;

	//this is map<tuple<all input params string representation>, output>. I make string representation from all inputs and compare it to strings in map
	map<string, vector<unordered_map<App::Uav, shared_ptr<Point>, UavHasher>>> cache;	//for same inputs are always same outputs. There will be stored inputs and their outputs.
	bool isInCache(string stringRepresentation);
	string argumentsToString(int distance_of_new_nodes, double max_turn, vector<shared_ptr<Uav>> uavs);
};

}
