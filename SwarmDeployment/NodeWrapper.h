#pragma once
#include "Node.h"
#include <set>


namespace AStar
{

	class NodeWrapper
	{
	public:
		NodeWrapper();
		virtual ~NodeWrapper();
		NodeWrapper(NodeWrapper* parent, App::Node* node, App::Node* endNode) :
			parent(parent), node(node)
		{
			this->fromParent = getDistance(parent);
			this->fromStart = this->parent->getFromStart() + this->fromParent;
			this->heuristicToEnd = 0;
			this->recalculateHeuristic(endNode);
			this->totalCost = this->getFromStart() + this->getHeuristicToEnd();
		}

		double getDistance(NodeWrapper* node) {
			double xDist = this->getX() - node->getX();
			xDist *= xDist;
			double yDist = this->getY() - node->getY();
			yDist *= yDist;
			return std::sqrt(xDist + yDist);
		}

		double getDistance(App::Node* node) {
			double xDist = this->getX() - node->getPoint()->getX();
			xDist *= xDist;
			double yDist = this->getY() - node->getPoint()->getY();
			yDist *= yDist;
			return std::sqrt(xDist + yDist);
		}

		std::set<NodeWrapper*> expand(App::Node* endNode) {
			auto expanded = this->node->getNeighbors();
			std::set<NodeWrapper*> expandedWrapper = std::set<NodeWrapper*>();
			for (App::Node* node : expanded) {
				expandedWrapper.insert(new NodeWrapper(this, node, endNode));
			}
			return expandedWrapper;
		}

		void recalculateHeuristic(App::Node* endNode) {
			this->heuristicToEnd = getDistance(endNode);
		}


		NodeWrapper* getParent() const {
			return parent;
		}

		App::Node* getNode() const {
			return node;
		}

		double getX() {
			return node->getPoint()->getX();
		}

		double getY() {
			return node->getPoint()->getY();
		}

		double getFromStart() {
			return fromStart;
		}

		double getHeuristicToEnd() {
			return heuristicToEnd;
		}

		bool hasParent() {
			return this->parent != nullptr;
		}
		//TODO: implementovat metodu equals
		std::set<NodeWrapper*> getWay() {
			std::set<NodeWrapper*> wayToTarget = std::set<NodeWrapper*>();
			NodeWrapper* iter = this;
			while (iter->hasParent()) {
				wayToTarget.insert(iter);
				iter = iter->getParent();
			}
			wayToTarget.insert(iter);       //pøidání poèátku
			return wayToTarget;
		}

		int getNodeId() {
			return 0;
		}

		double getTotalCost() {
			return totalCost;
		}

		bool operator<(const NodeWrapper& another); //needed for comparation when using count method of std::set
		bool operator>(const NodeWrapper& another); //If nodes are same, wrappers should also behave as same.
		bool operator==(const NodeWrapper& another);
		bool operator!=(const NodeWrapper& another);

	protected:
		NodeWrapper* parent;
		App::Node* node;
		double fromStart;
		double fromParent;
		double heuristicToEnd;
		double totalCost;
	};

}