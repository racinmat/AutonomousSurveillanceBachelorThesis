#pragma once
#include "LoggerInterface.h"
#include "memory"

using namespace std;

namespace App
{

	class Configuration;

	class Core : public enable_shared_from_this<Core>
	{
	
	public:
		explicit Core(shared_ptr<Configuration> configuration);
		virtual ~Core();
		void run();
		void setLogger(shared_ptr<LoggerInterface> logger);
		void logConfigurationChange();

	protected:
		shared_ptr<LoggerInterface> logger;
		vector<shared_ptr<Map>> maps;
		shared_ptr<Configuration> configuration;
	};

}

