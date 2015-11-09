#include "NodeWrapper.h"
#include <string>
#include <iostream>


namespace AStar
{
	int NodeWrapper::lastId = 0;

	NodeWrapper::~NodeWrapper()
	{
		std::cout << "Deleting node with coords: " << coords << ", and id: "<< id << std::endl ;
	}

	NodeWrapper::NodeWrapper(std::shared_ptr<NodeWrapper> parent, std::shared_ptr<App::Node> node, std::shared_ptr<App::Node> endNode) :
		parent(parent), node(node), pathLength(parent == nullptr ? 1 : parent->pathLength + 1), id(lastId++)
	{
		if (parent == nullptr)
		{
			this->fromParent = 0;
			this->fromStart = 0;
		} else
		{
			this->fromParent = getDistance(parent);
			this->fromStart = this->parent->getFromStart() + this->fromParent;
			this->fromStart *= this->parent->node->getCost();//zde zahrnu do výpoètù cenu podle vzdálenosti od pøekážek. Petrlík cenou hodnotu fromStart násobil, ale pøijde mi logiètìjší pøièítání. Vytkouším pøièítání pozdìji
		}
		this->coords = std::string("x:") + std::to_string(node->getPoint()->getX()) + std::string(",y:") + std::to_string(node->getPoint()->getY());
		this->heuristicToEnd = 0;
		this->recalculateHeuristic(endNode);
		this->totalCost = this->getFromStart() + this->getHeuristicToEnd();
	}

	double NodeWrapper::getDistance(std::shared_ptr<NodeWrapper> node)
	{
		double xDist = this->getX() - node->getX();
		xDist *= xDist;
		double yDist = this->getY() - node->getY();
		yDist *= yDist;
		return std::sqrt(xDist + yDist);
	}

	double NodeWrapper::getDistance(std::shared_ptr<App::Node> node)
	{
		double xDist = this->getX() - node->getPoint()->getX();
		xDist *= xDist;
		double yDist = this->getY() - node->getPoint()->getY();
		yDist *= yDist;
		return std::sqrt(xDist + yDist);
	}

	std::set<std::shared_ptr<NodeWrapper>> NodeWrapper::expand(std::shared_ptr<App::Node> endNode)
	{
		auto expanded = this->node->getNeighbors();
		auto expandedWrapper = std::set<std::shared_ptr<NodeWrapper>>();
		for (auto node : expanded) {
			expandedWrapper.insert(std::make_shared<NodeWrapper>(this->getPointer(), node, endNode));
		}
		return expandedWrapper;
	}

	void NodeWrapper::recalculateHeuristic(std::shared_ptr<App::Node> endNode)
	{
		this->heuristicToEnd = getDistance(endNode);
	}

	std::shared_ptr<NodeWrapper> NodeWrapper::getParent() const
	{
		return parent;
	}

	std::shared_ptr<App::Node> NodeWrapper::getNode() const
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

	std::vector<std::shared_ptr<NodeWrapper>> NodeWrapper::getWay()
	{
		auto wayToTarget = std::vector<std::shared_ptr<NodeWrapper>>(pathLength);
		auto iter = std::shared_ptr<NodeWrapper>(this);
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

	bool NodeWrapper::operator==(const App::Node& another)
	{
		return  getX() == another.getPoint()->getX() && getY() == another.getPoint()->getY();
	}

	bool NodeWrapper::operator!=(const NodeWrapper& another)
	{
		return !(*this == another);
	}

	bool NodeWrapper::operator!=(const App::Node& another)
	{
		return !(*this == another);
	}

	std::shared_ptr<NodeWrapper> NodeWrapper::getPointer()
	{
		return shared_from_this();
	}
}