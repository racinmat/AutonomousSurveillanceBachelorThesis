#pragma once
#include "Path.h"
#include "Map.h"

namespace App
{

	class GuidingPathFactory
	{
	public:
		GuidingPathFactory();
		std::vector<App::Path*> createGuidingPaths(App::Map* map) const;
	};

}
