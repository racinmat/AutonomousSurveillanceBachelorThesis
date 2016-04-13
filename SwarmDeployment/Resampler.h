#pragma once
#include <memory>
#include <vector>
#include "StateFactory.h"
#include "MotionModel.h"

using namespace std;

namespace App
{
	class Configuration;

	class Resampler
	{
	public:
		Resampler(shared_ptr<Configuration> configuration, shared_ptr<StateFactory> stateFactory, shared_ptr<MotionModel> motionModel);
		vector<shared_ptr<State>> resampleToMaxFrequency(vector<shared_ptr<State>> path);	//resamples the path to max frequency, so the new path will be more precise and smooth
		double getNearestNextTimeOfOldPath(double time);	//returns time of next state of old path
		virtual ~Resampler();
		virtual double getNewFrequency() const;

	private:
		shared_ptr<Configuration> configuration;
		shared_ptr<StateFactory> stateFactory;
		shared_ptr<MotionModel> motionModel;

		double newFrequency;
	};


}
