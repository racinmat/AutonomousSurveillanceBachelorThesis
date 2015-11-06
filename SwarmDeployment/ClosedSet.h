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
		virtual bool contains(NodeWrapper* nodeWrapper) override;
	};

}