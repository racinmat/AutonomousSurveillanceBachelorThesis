#pragma once
#include <memory>
#include "Configuration.h"

using namespace std;

namespace App
{
	class Resampler
	{
	public:
		Resampler(shared_ptr<Configuration> configuration);
		vector<shared_ptr<State>> resampleToMaxFrequency(vector<shared_ptr<State>> path);	//resamples the path to max frequency, so the new path will be more precise and smooth
		double getNearestNextTimeOfOldPath(double time);	//returns time of next state of old path
		virtual ~Resampler();
	private:
		shared_ptr<Configuration> configuration;
	};


}
