#include "Resampler.h"
#include "Configuration.h"

Resampler::Resampler(shared_ptr<Configuration> configuration, shared_ptr<StateFactory> stateFactory, shared_ptr<MotionModel> motionModel) : 
	configuration(configuration), stateFactory(stateFactory), motionModel(motionModel)
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
	double newFrequency = min<double>(maxAvailableFrequency, maxFrequency);	//pokud je maxFrequency v�t�� ne� maxAvailableFrequency, bude nastavena maxAvailable, jinak maaxFrequency
	int ratio = floor(newFrequency / currentFrequency);	//bude ratio kr�t v�ce vzork�
	double newStepSize = configuration->getDistanceOfNewNodes() / ratio;

	//resampling will be only to more samples, because I do not need lower resolution
	//resampling will be only by whole numbers, it is sufficient

	vector<shared_ptr<State>> newPath = vector<shared_ptr<State>>();
	shared_ptr<State> previous;
	shared_ptr<State> next;
	for (size_t i = 1; i < path.size(); i++)
	{
		previous = path[i - 1];
		next = path[i];
		newPath.push_back(stateFactory->createState(*previous));
		for (size_t j = 1; j < ratio - 1; j++)
		{
			shared_ptr<State> newState = stateFactory->createState(*previous);
			for (auto uav : newState->getUavs())
			{
				auto input = next->getUav(uav)->getPreviousInput();
				input.setStep(newStepSize);
				motionModel->calculateState(uav, input);
			}
			newPath.push_back(newState);
			previous = newState;
		}
		newPath.push_back(stateFactory->createState(*next));
	}
	return newPath;
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
