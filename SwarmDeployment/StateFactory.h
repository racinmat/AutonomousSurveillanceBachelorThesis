#pragma once
#include "Configuration.h"

using namespace std;

namespace App
{
	class StateFactory
	{
	public:
		explicit StateFactory(shared_ptr<Configuration> configuration);
		~StateFactory();
		shared_ptr<State> createState();

	protected:
		shared_ptr<Configuration> configuration;
	};


}
