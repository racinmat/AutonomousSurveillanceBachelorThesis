#pragma once
#include <memory>

using namespace std;

namespace App
{
	class State;
	class LinkedState;
	class Configuration;

	class StateFactory
	{
	public:
		explicit StateFactory(shared_ptr<Configuration> configuration);
		~StateFactory();
		shared_ptr<LinkedState> createState();
		shared_ptr<LinkedState> createState(const LinkedState& other);
		shared_ptr<State> createState(const State& other);

	protected:
		shared_ptr<Configuration> configuration;
	};


}
