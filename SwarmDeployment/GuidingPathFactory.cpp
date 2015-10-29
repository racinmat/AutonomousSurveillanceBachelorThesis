#include "GuidingPathFactory.h"

namespace App
{
	App::GuidingPathFactory::GuidingPathFactory()
	{
	}

	std::vector<App::Path*> GuidingPathFactory::createGuidingPaths(App::Map* map) const
	{
		std::vector<App::Path*> paths = std::vector<App::Path*>(map->countGoals());
		return paths;
	}

}
