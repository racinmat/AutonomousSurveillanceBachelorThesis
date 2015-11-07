#include "NodeSet.h"

namespace AStar
{

	bool NodeSet::contains(NodeWrapper* node)
	{
//		return count(node) > 0;	//nefunguje pro pointery, pokud chci vlastní porovnávání
		for (NodeWrapper* another : (*this))
		{
			if ((*another) == (*node))
			{
				return true;
			}
		}
		return false;
	}

	NodeWrapper* NodeSet::find(NodeWrapper* node)
	{
		for (NodeWrapper* another : (*this))
		{
			if ((*another) == (*node))
			{
				return another;
			}
		}
		return nullptr;
	}
}