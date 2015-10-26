#include <iostream>
#include "../include/Map.h"
#include "../include/MapFactory.h"
#include "../include/Configuration.h"

int main(int argc, char *argv[])
{
	std::cout << "test";

	App::MapFactory* mapFactory = new App::MapFactory();
	std::vector<App::Map*> maps = mapFactory->createMaps();
	App::Configuration* configuration = new App::Configuration();

	App::Map* map = maps.at(configuration->mapNumber);


	delete mapFactory;
	return 0;
}
