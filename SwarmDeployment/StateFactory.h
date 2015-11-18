#pragma once
#include <memory>

using namespace std;

namespace App
{
	class State;
	class Configuration;

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
