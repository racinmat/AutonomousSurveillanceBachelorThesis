#pragma once
#include <set>
#include "NodeWrapper.h"

namespace AStar
{
	struct comparator {
//		bool operator() (const NodeWrapper& left, const NodeWrapper& right) const
//		{
//			return  (*left) < (*right);
//		}
	};

	class NodeSet : public std::set<NodeWrapper*>
	{
	public:
		virtual bool contains(NodeWrapper* node);
	};


}