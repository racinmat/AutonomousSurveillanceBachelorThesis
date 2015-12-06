#include "StateFactory.h"
#include "Configuration.h"
#include "State.h"

namespace App
{


	StateFactory::StateFactory(shared_ptr<Configuration> configuration) : configuration(configuration)
	{
	}

	StateFactory::~StateFactory()
	{
	}

	shared_ptr<State> StateFactory::createState()
	{
		return make_shared<State>(configuration->getInputCount());
	}

	shared_ptr<State> StateFactory::createState(const State& other)
	{
		return make_shared<State>(other);
	}

}
