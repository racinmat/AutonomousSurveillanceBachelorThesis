#pragma once
#include <memory>

using namespace std;

namespace App
{
	class LinkedState;
	class Configuration;

	class StateFactory
	{
	public:
		explicit StateFactory(shared_ptr<Configuration> configuration);
		~StateFactory();
		shared_ptr<LinkedState> createState();
		shared_ptr<LinkedState> createState(const LinkedState& other);

	protected:
		shared_ptr<Configuration> configuration;
	};


}
