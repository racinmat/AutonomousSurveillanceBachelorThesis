#include "OpenedSet.h"

namespace AStar
{


	OpenedSet::OpenedSet()
	{
	}


	OpenedSet::~OpenedSet()
	{
	}

	bool OpenedSet::contains(NodeWrapper* node)
	{
		if (NodeSet::contains(node)) {// when some node is in opened list, but same node is later found in shorter path, I need to switch these nodes, so node with shorter path would be in opened list instead of node with longer path.
			auto iterator = find(node); //získám stejnou node, do které jsem pøišel odjinud a porovnám délky
			NodeWrapper* another = (*iterator);
			if (another->getTotalCost() > node->getTotalCost()) {
				remove(another);
				insert(node);
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