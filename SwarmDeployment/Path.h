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
		virtual ~Path();
		std::list<std::shared_ptr<Node>> getNodes() const;
		int getSize() const;
		void addToEnd(std::shared_ptr<Node> node);
		void addToStart(std::shared_ptr<Node> node);

	protected:
		std::list<std::shared_ptr<Node>> nodes;
	};

}