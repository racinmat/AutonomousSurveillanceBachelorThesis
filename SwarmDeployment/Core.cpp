#include "Core.h"
#include "Map.h"
#include "Configuration.h"
#include "MapFactory.h"

namespace App
{

	Core::Core(Configuration* configuration) :
		configuration(configuration)
	{
		setLogger(new App::LoggerInterface());	//I will use LoggerInterface as NilObject for Logger, because I am too lazy to write NilObject Class.
		configuration->setCore(this);

		App::MapFactory* mapFactory = new App::MapFactory();
		maps = mapFactory->createMaps();
		delete mapFactory;
	}


	Core::~Core()
	{
	}

	void Core::run()
	{
		App::Map* map = maps.at(configuration->getMapNumber());

		logger->logSelectedMap(map);

	}

	void Core::setLogger(App::LoggerInterface* logger)
	{
		this->logger = logger;
	}

	void Core::logConfigurationChange()
	{
		App::Map* map = maps.at(configuration->getMapNumber());
		logger->logSelectedMap(map);
	}

}
