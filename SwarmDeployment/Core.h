#pragma once
#include "LoggerInterface.h"

namespace App
{

	class Configuration;

	class Core
	{
	
	public:
		Core(Configuration* configuration);
		~Core();
		void run();
		void setLogger(LoggerInterface* logger);
		void logConfigurationChange();

	protected:
		LoggerInterface* logger;
		std::vector<Map*> maps;
		Configuration* configuration;
	};

}

