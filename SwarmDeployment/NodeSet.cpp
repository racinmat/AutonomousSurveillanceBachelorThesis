#include "NodeSet.h"

namespace AStar
{
	NodeSet::~NodeSet()
	{
	}

	bool NodeSet::contains(std::shared_ptr<NodeWrapper> node)
	{
//		return count(node) > 0;	//nefunguje pro pointery, pokud chci vlastní porovnávání
		for (auto another : (*this))
		{
			if ((*another.get()) == (*node.get()))
			{
				return true;
			}
		}
		return false;
	}

	std::shared_ptr<NodeWrapper> NodeSet::find(std::shared_ptr<NodeWrapper> node)
	{
		for (auto another : (*this))
		{
			if ((*another.get()) == (*node.get()))
			{
				return another;
			}
		}
		return nullptr;
	}
}