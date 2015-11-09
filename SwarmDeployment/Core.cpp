#include "Core.h"
#include "Map.h"
#include "Configuration.h"
#include "MapFactory.h"
#include "Path.h"
#include "GuidingPathFactory.h"
#include "MapProcessor.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <memory>

namespace App
{

	Core::Core(Configuration* configuration) :
		configuration(configuration), logger(nullptr)
	{
		setLogger(new App::LoggerInterface());	//I will use LoggerInterface as NilObject for Logger, because I am too lazy to write NilObject Class.
		configuration->setCore(this);

		App::MapFactory mapFactory;
		maps = mapFactory.createMaps(configuration->getUavCount());
	}


	Core::~Core()
	{
	}

	void Core::run()
	{
		std::clock_t start;
		double duration;

		start = std::clock();

		std::shared_ptr<App::Map> map = maps.at(configuration->getMapNumber());
		logger->logSelectedMap(map, configuration->getWorldWidth(), configuration->getWorldHeight());
		MapProcessor mapProcessor = MapProcessor(logger);
		auto nodes = mapProcessor.mapToNodes(map, configuration->getAStarCellSize(), configuration->getWorldWidth(), configuration->getWorldHeight(), configuration->getUavSize());
		GuidingPathFactory pathFactory = GuidingPathFactory(logger);
		std::vector<std::shared_ptr<Path>> paths = pathFactory.createGuidingPaths(nodes->getAllNodes(), nodes->getStartNode(), nodes->getEndNodes());
		
		delete nodes;

		//here comes RRT-Path.

		duration = (std::clock() - start) / double(CLOCKS_PER_SEC);

		logger->logText(std::to_string(duration) + "sekund");

	}

	void Core::setLogger(App::LoggerInterface* logger)
	{
		this->logger = logger;
	}

	void Core::logConfigurationChange()
	{
		auto map = maps.at(configuration->getMapNumber());
		logger->logSelectedMap(map, configuration->getWorldWidth(), configuration->getWorldHeight());
	}



}
