#pragma once
#include "NodeSet.h"

namespace AStar
{
	class NodeWrapper;

	class ClosedSet : public NodeSet
	{
	public:
		ClosedSet();
		virtual ~ClosedSet();
		virtual bool contains(std::shared_ptr<NodeWrapper> nodeWrapper) override;
	};

}