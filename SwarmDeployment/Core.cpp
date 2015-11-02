#include "Core.h"
#include "Map.h"
#include "Configuration.h"
#include "MapFactory.h"
#include "Path.h"
#include "GuidingPathFactory.h"
#include "MapProcessor.h"

namespace App
{

	Core::Core(Configuration* configuration) :
		configuration(configuration)
	{
		setLogger(new App::LoggerInterface());	//I will use LoggerInterface as NilObject for Logger, because I am too lazy to write NilObject Class.
		configuration->setCore(this);

		App::MapFactory* mapFactory = new App::MapFactory();
		maps = mapFactory->createMaps(configuration->getUavCount());
		delete mapFactory;
	}


	Core::~Core()
	{
	}

	void Core::run()
	{
		App::Map* map = maps.at(configuration->getMapNumber());
		logger->logSelectedMap(map, configuration->getWorldWidth(), configuration->getWorldHeight());

		MapProcessor* mapProcessor = new MapProcessor(logger);
		auto nodes = mapProcessor->mapToNodes(map, configuration->getAStarCellSize(), configuration->getWorldWidth(), configuration->getWorldHeight(), configuration->getUavSize());
		GuidingPathFactory* pathFactory = new GuidingPathFactory(logger);
//		std::vector<Path*> paths = pathFactory->createGuidingPaths(nodes, start, end);

	}

	void Core::setLogger(App::LoggerInterface* logger)
	{
		this->logger = logger;
	}

	void Core::logConfigurationChange()
	{
		App::Map* map = maps.at(configuration->getMapNumber());
		logger->logSelectedMap(map, configuration->getWorldWidth(), configuration->getWorldHeight());
	}

}
