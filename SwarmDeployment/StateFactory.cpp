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

	shared_ptr<LinkedState> StateFactory::createState()
	{
		return make_shared<LinkedState>(configuration->getInputCount());
	}

	shared_ptr<LinkedState> StateFactory::createState(const LinkedState& other)
	{
		return make_shared<LinkedState>(other);
	}

}
