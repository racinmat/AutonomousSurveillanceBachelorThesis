#pragma once
#include <vector>
#include "Node.h"
#include <list>

namespace App
{

	class Path
	{
	public:
		Path();
		std::list<App::Node*> getNodes() const;
		void addToEnd(App::Node* node);
		void addToStart(App::Node* node);

	protected:
		std::list<App::Node*> nodes;
	};

}