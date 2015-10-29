#pragma once
#include <vector>
#include "Node.h"

namespace App
{

	class Path
	{
	public:
		Path();
		std::vector<App::Node*> getNodes() const;
		void addNode(App::Node* node);

	protected:
		std::vector<App::Node*> nodes;
	};

}