#pragma once
#include "NodeSet.h"

namespace AStar
{
	class OpenedSet : public NodeSet
	{
	public:
		OpenedSet();
		virtual ~OpenedSet();
		virtual bool contains(NodeWrapper* nodeWrapper) override;
	};

}