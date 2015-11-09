#pragma once
#include "LoggerInterface.h"
#include "memory"

namespace App
{

	class Configuration;

	class Core
	{
	
	public:
		explicit Core(Configuration* configuration);
		virtual ~Core();
		void run();
		void setLogger(LoggerInterface* logger);
		void logConfigurationChange();

	protected:
		LoggerInterface* logger;
		std::vector<std::shared_ptr<App::Map>> maps;
		Configuration* configuration;
	};

}

