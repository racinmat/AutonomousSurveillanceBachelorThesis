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
		~Path();
		std::list<std::shared_ptr<App::Node>> getNodes() const;
		void addToEnd(std::shared_ptr<App::Node> node);
		void addToStart(std::shared_ptr<App::Node> node);

	protected:
		std::list<std::shared_ptr<App::Node>> nodes;
	};

}