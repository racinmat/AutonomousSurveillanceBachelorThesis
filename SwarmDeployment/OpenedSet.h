#pragma once
#include "NodeSet.h"

namespace AStar
{
	class OpenedSet : public NodeSet
	{
	public:
		OpenedSet();
		virtual ~OpenedSet();
		virtual bool contains(std::shared_ptr<NodeWrapper> nodeWrapper) override;
		std::shared_ptr<NodeWrapper> pollBest();

	protected:
		std::shared_ptr<NodeWrapper> getBest();
	};

}