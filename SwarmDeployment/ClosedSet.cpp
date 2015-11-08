#include "ClosedSet.h"

namespace AStar
{
	ClosedSet::ClosedSet()
	{
	}


	ClosedSet::~ClosedSet()
	{
	}

	bool ClosedSet::contains(std::shared_ptr<NodeWrapper> node)
	{
		if (NodeSet::contains(node)) {// when some node is in closed list, but same node is later found in shorter path, I need to remove that node from closed list.
			auto another = find(node); //získám stejnou node, do které jsem pøišel odjinud a porovnám délky
			if (another->getTotalCost() > node->getTotalCost()) {
				erase(another);
				return false;
			}
			else {
				return true;
			}
		}
		else {
			return false;
		}
	}
}