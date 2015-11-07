#include "NodeWrapper.h"
#include <string>


namespace AStar
{

	NodeWrapper::~NodeWrapper()
	{
	}

	NodeWrapper::NodeWrapper(NodeWrapper* parent, App::Node* node, App::Node* endNode):
		parent(parent), node(node), pathLength(parent == nullptr ? 1 : parent->pathLength + 1)
	{
		if (parent == nullptr)
		{
			this->fromParent = 0;
			this->fromStart = 0;
		} else
		{
			this->fromParent = getDistance(parent);
			this->fromStart = this->parent->getFromStart() + this->fromParent;
		}
		this->coords = std::string("x:") + std::to_string(node->getPoint()->getX()) + std::string(",y:") + std::to_string(node->getPoint()->getY());
		this->heuristicToEnd = 0;
		this->recalculateHeuristic(endNode);
		this->totalCost = this->getFromStart() + this->getHeuristicToEnd();
	}

	double NodeWrapper::getDistance(NodeWrapper* node)
	{
		double xDist = this->getX() - node->getX();
		xDist *= xDist;
		double yDist = this->getY() - node->getY();
		yDist *= yDist;
		return std::sqrt(xDist + yDist);
	}

	double NodeWrapper::getDistance(App::Node* node)
	{
		double xDist = this->getX() - node->getPoint()->getX();
		xDist *= xDist;
		double yDist = this->getY() - node->getPoint()->getY();
		yDist *= yDist;
		return std::sqrt(xDist + yDist);
	}

	std::set<NodeWrapper*> NodeWrapper::expand(App::Node* endNode)
	{
		auto expanded = this->node->getNeighbors();
		std::set<NodeWrapper*> expandedWrapper = std::set<NodeWrapper*>();
		for (App::Node* node : expanded) {
			expandedWrapper.insert(new NodeWrapper(this, node, endNode));
		}
		return expandedWrapper;
	}

	void NodeWrapper::recalculateHeuristic(App::Node* endNode)
	{
		this->heuristicToEnd = getDistance(endNode);
	}

	NodeWrapper* NodeWrapper::getParent() const
	{
		return parent;
	}

	App::Node* NodeWrapper::getNode() const
	{
		return node;
	}

	double NodeWrapper::getX() const
	{
		return node->getPoint()->getX();
	}

	double NodeWrapper::getY() const
	{
		return node->getPoint()->getY();
	}

	double NodeWrapper::getFromStart() const
	{
		return fromStart;
	}

	double NodeWrapper::getHeuristicToEnd() const
	{
		return heuristicToEnd;
	}

	bool NodeWrapper::hasParent()
	{
		return this->parent != nullptr;
	}

	std::vector<NodeWrapper*> NodeWrapper::getWay()
	{
		std::vector<NodeWrapper*> wayToTarget = std::vector<NodeWrapper*>(pathLength);
		NodeWrapper* iter = this;
		int index = pathLength;
		while (iter->hasParent()) {
			index--;
			wayToTarget[index] = iter;
			iter = iter->getParent();
		}
		index--;
		wayToTarget[index] = iter;       //pøidání poèátku
		return wayToTarget;
	}

	double NodeWrapper::getTotalCost()
	{
		return totalCost;
	}

	bool NodeWrapper::operator==(const NodeWrapper& another)
	{
		return  getX() == another.getX() && getY() == another.getY();
	}

	bool NodeWrapper::operator!=(const NodeWrapper& another)
	{
		return !(*this == another);
	}
}