#include "InputGenerator.h"
#include <string>

namespace App {

	InputGenerator::InputGenerator(int input_samples_dist, int input_samples_phi) : 
		input_samples_dist(input_samples_dist), input_samples_phi(input_samples_phi)
	{
	}

	InputGenerator::~InputGenerator()
	{
	}

	vector<unordered_map<Uav, shared_ptr<Point>, UavHasher>> InputGenerator::generateAllInputs(int distance_of_new_nodes, double max_turn, vector<shared_ptr<Uav>> uavs)
	{
		string stringRepresentation = argumentsToString(distance_of_new_nodes, max_turn, uavs);
		if (isInCache(stringRepresentation))
		{
			return cache[stringRepresentation];
		}

		auto oneUavInputs = generateOneUavInputs(distance_of_new_nodes, max_turn);
		auto inputs = generator.generateNTuplet(oneUavInputs, uavs);		//poèet všech kombinací je poèet všech možných vstupù jednoho UAV ^ poèet UAV
		cache[stringRepresentation] = inputs;
		return inputs;
	}

	vector<shared_ptr<Point>> InputGenerator::generateOneUavInputs(int distance_of_new_nodes, double max_turn)
	{
		vector<shared_ptr<Point>> oneUavInputs = vector<shared_ptr<Point>>();

		for (size_t k = 0; k < input_samples_dist; k++)
		{
			for (size_t m = 0; m < input_samples_phi; m++)
			{
				double x = distance_of_new_nodes / pow(1.5, k);
				double y = -max_turn + 2 * m * max_turn / (input_samples_phi - 1);
				shared_ptr<Point> point = make_shared<Point>(x, y);
				oneUavInputs.push_back(point);
			}
		}

		return oneUavInputs;
	}

	bool InputGenerator::isInCache(string stringRepresentation)
	{
		return cache.find(stringRepresentation) != cache.end();
	}

	string InputGenerator::argumentsToString(int distance_of_new_nodes, double max_turn, vector<shared_ptr<Uav>> uavs)
	{
		string string = "d:" + to_string(distance_of_new_nodes) + "m:" + to_string(max_turn);
		for (auto uav : uavs)
		{
			string += "id:" + to_string(uav->getId());
		}
		return string;
	}

}
