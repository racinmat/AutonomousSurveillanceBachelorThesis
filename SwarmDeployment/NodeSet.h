#pragma once
#include <set>
#include "NodeWrapper.h"

namespace AStar
{

	class NodeSet : public std::set<NodeWrapper*>
	{
	public:
		virtual bool contains(NodeWrapper* node);
	};

}