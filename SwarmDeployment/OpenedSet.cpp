#include "OpenedSet.h"

namespace AStar
{


	OpenedSet::OpenedSet()
	{
	}


	OpenedSet::~OpenedSet()
	{
	}

	bool OpenedSet::contains(std::shared_ptr<NodeWrapper> node)
	{
		if (NodeSet::contains(node)) {// when some node is in opened list, but same node is later found in shorter path, I need to switch these nodes, so node with shorter path would be in opened list instead of node with longer path.
			auto another = find(node); //získám stejnou node, do které jsem pøišel odjinud a porovnám délky
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

	std::shared_ptr<NodeWrapper> OpenedSet::pollBest()
	{
		auto best = getBest();
		erase(best);
		return best;
	}

	std::shared_ptr<NodeWrapper> OpenedSet::getBest()
	{
		double best = DBL_MAX;
		std::shared_ptr<NodeWrapper> bestNode = nullptr;
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