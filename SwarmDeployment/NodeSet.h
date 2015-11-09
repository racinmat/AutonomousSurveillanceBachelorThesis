#pragma once
#include <set>
#include "NodeWrapper.h"
#include <memory>

namespace AStar
{

	class NodeSet : public std::set<std::shared_ptr<NodeWrapper>>
	{
	public:
		virtual ~NodeSet();
		virtual bool contains(std::shared_ptr<NodeWrapper> node);
		virtual std::shared_ptr<NodeWrapper> find(std::shared_ptr<NodeWrapper> node);
	};


}