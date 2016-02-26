#include "Resampler.h"


Resampler::Resampler(shared_ptr<Configuration> configuration) : configuration(configuration)
{
}

vector<shared_ptr<State>> Resampler::resampleToMaxFrequency(vector<shared_ptr<State>> path)
{
	int sampleCount = path.size();
	double totalTime = configuration->getEndTime() * sampleCount;	//total time to fly the path
	double currentFrequency = sampleCount / totalTime;	//samples per second
	double maxFrequency = configuration->getMaxSampleFrequency();
	double maxSampleCount = configuration->getMaxSampleCount();
	int maxAvailableFrequency = maxSampleCount / totalTime;
	double newFrequency = min<double>(maxAvailableFrequency, maxFrequency);	//pokud je maxFrequency vìtší než maxAvailableFrequency, bude nastavena maxAvailable, jinak maaxFrequency
	double ratio = newFrequency / currentFrequency;	//bude ratio krát více vzorkù
	double newTimeStep = 1 / newFrequency;

	vector<shared_ptr<State>> newPath = vector<shared_ptr<State>>();
	for (size_t currentTime = 0; currentTime < totalTime; currentTime += )	//promìnnou currentTime pojedu od 0 do konce a oèekávám, že pøevzorkováním trajektorii témìø nezmìním, UAV bude ve stejný èas na stejném místì
	{
	}
}

Resampler::~Resampler()
{
}
