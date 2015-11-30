#include "NodeWrapper.h"
#include <string>
#include <iostream>


namespace AStar
{

	NodeWrapper::~NodeWrapper()
	{
	}

	NodeWrapper::NodeWrapper(shared_ptr<NodeWrapper> parent, shared_ptr<Node> node, shared_ptr<Node> endNode) :
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
			this->fromStart += this->parent->node->getCost();//zde zahrnu do výpoètù cenu podle vzdálenosti od pøekážek. Petrlík cenou hodnotu fromStart násobil, ale pøijde mi logiètìjší pøièítání. Vytkouším pøièítání pozdìji
		}
		this->coords = string("x:") + to_string(node->getPoint()->getX()) + string(",y:") + to_string(node->getPoint()->getY());
		this->heuristicToEnd = 0;
		this->recalculateHeuristic(endNode);
		this->totalCost = this->getFromStart() + this->getHeuristicToEnd();
	}

	double NodeWrapper::getDistance(shared_ptr<NodeWrapper> node)
	{
		double xDist = this->getX() - node->getX();
		xDist *= xDist;
		double yDist = this->getY() - node->getY();
		yDist *= yDist;
		return sqrt(xDist + yDist);
	}

	double NodeWrapper::getDistance(shared_ptr<Node> node)
	{
		double xDist = this->getX() - node->getPoint()->getX();
		xDist *= xDist;
		double yDist = this->getY() - node->getPoint()->getY();
		yDist *= yDist;
		return sqrt(xDist + yDist);
	}

	set<shared_ptr<NodeWrapper>> NodeWrapper::expand(shared_ptr<Node> endNode)
	{
		auto expanded = this->node->getNeighborsWithoutObstacles();
		auto expandedWrapper = set<shared_ptr<NodeWrapper>>();
		for (auto node : expanded) {
			expandedWrapper.insert(make_shared<NodeWrapper>(shared_from_this(), node, endNode));
		}
		return expandedWrapper;
	}

	void NodeWrapper::recalculateHeuristic(shared_ptr<Node> endNode)
	{
		this->heuristicToEnd = getDistance(endNode);
	}

	shared_ptr<NodeWrapper> NodeWrapper::getParent() const
	{
		return parent;
	}

	shared_ptr<Node> NodeWrapper::getNode() const
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

	vector<shared_ptr<NodeWrapper>> NodeWrapper::getWay()
	{
		auto wayToTarget = vector<shared_ptr<NodeWrapper>>(pathLength);
		auto iter = shared_from_this();
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

	bool NodeWrapper::operator==(const Node& another)
	{
		return  getX() == another.getPoint()->getX() && getY() == another.getPoint()->getY();
	}

	bool NodeWrapper::operator!=(const NodeWrapper& another)
	{
		return !(*this == another);
	}

	bool NodeWrapper::operator!=(const Node& another)
	{
		return !(*this == another);
	}

}