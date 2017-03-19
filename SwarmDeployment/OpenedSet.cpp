#include <cfloat>
#include "OpenedSet.h"

namespace AStar
{


	OpenedSet::OpenedSet()
	{
	}


	OpenedSet::~OpenedSet()
	{
	}

	bool OpenedSet::contains(shared_ptr<NodeWrapper> node)
	{
		if (NodeSet::contains(node)) {// when some node is in opened list, but same node is later found in shorter path, I need to switch these nodes, so node with shorter path would be in opened list instead of node with longer path.
			auto another = find(node); //z�sk�m stejnou node, do kter� jsem p�i�el odjinud a porovn�m d�lky
			if (another->getTotalCost() > node->getTotalCost()) {
				erase(another);
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

	shared_ptr<NodeWrapper> OpenedSet::pollBest()
	{
		auto best = getBest();
		erase(best);
		return best;
	}

	shared_ptr<NodeWrapper> OpenedSet::getBest()
	{
		double best = DBL_MAX;
		shared_ptr<NodeWrapper> bestNode = nullptr;
		for (auto node : (*this))
		{
			if (node->getTotalCost() < best)
			{
				best = node->getTotalCost();
				bestNode = node;
			}

		}
		return bestNode;
	}
}