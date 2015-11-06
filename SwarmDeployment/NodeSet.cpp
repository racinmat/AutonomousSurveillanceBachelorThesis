#include "NodeSet.h"

namespace AStar
{

	bool NodeSet::contains(NodeWrapper* node)
	{
		return count(node) > 0;
	}

}