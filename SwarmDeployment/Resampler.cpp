#include "Resampler.h"


Resampler::Resampler(shared_ptr<Configuration> configuration) : configuration(configuration)
{
}

vector<shared_ptr<State>> Resampler::resampleToMaxFrequency(vector<shared_ptr<State>> path)
{
	double timeStep = configuration->getTimeStep();
	int sampleCount = path.size();
	double totalTime = timeStep * sampleCount;	//total time to fly the path
	double currentFrequency = sampleCount / totalTime;	//samples per second
	double maxFrequency = configuration->getMaxSampleFrequency();
	double maxSampleCount = configuration->getMaxSampleCount();
	int maxAvailableFrequency = maxSampleCount / totalTime;
	double newFrequency = min<double>(maxAvailableFrequency, maxFrequency);	//pokud je maxFrequency vìtší než maxAvailableFrequency, bude nastavena maxAvailable, jinak maaxFrequency
	int ratio = floor(newFrequency / currentFrequency);	//bude ratio krát více vzorkù
	
	//resampling will be only to more samples, because I do not need lower resolution
	//resampling will be only by whole numbers, it is sufficient

	vector<shared_ptr<State>> newPath = vector<shared_ptr<State>>();
	newPath.push_back(make_shared<State>(*path[0]));
	for (size_t currentTime = newTimeStep; currentTime < totalTime; currentTime += newTimeStep)	//promìnnou currentTime pojedu od 0 do konce a oèekávám, že pøevzorkováním trajektorii témìø nezmìním, UAV bude ve stejný èas na stejném místì
	{
		double nextStateTime;
		int nextOldStateIndex = getNearestNextTimeOfOldPath(currentTime) / timeStep;
		shared_ptr<State> previousOldState = path[nextOldStateIndex - 1];
		for (auto uav : previousOldState->getUavs())
		{
//			uav->getPointParticle()->getDubinsManeuver();
//			geom::Dubins()
		}

	}
}

double Resampler::getNearestNextTimeOfOldPath(double time)
{
	double timeStep = configuration->getTimeStep();
	double iter = 0;
	while (iter < time)
	{
		iter += timeStep;
	}
	iter += timeStep;
	return iter;
}

Resampler::~Resampler()
{
}
