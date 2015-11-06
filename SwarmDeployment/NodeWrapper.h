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
			double xDistSquare = Math.pow((this.getX() - node.getX()), 2);
			double yDistSquare = Math.pow((this.getY() - node.getY()), 2);
			return Math.sqrt(xDistSquare + yDistSquare);
		}

		double getDistance(App::Node* node) {
			double xDistSquare = Math.pow((this.getX() - node.getX()), 2);
			double yDistSquare = Math.pow((this.getY() - node.getY()), 2);
			return Math.sqrt(xDistSquare + yDistSquare);
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


		NodeWrapper* getParent() {
			return parent;
		}

		App::Node* getNode() {
			return node;
		}

		double getX() {
			return node->getX();
		}

		double getY() {
			return node->getY();
		}

		bool isTarget() {
			return node->isTarget();
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
		std::vector<NodeWrapper*> getWay() {
			List<NodeWrapper> wayToTarget = new ArrayList<NodeWrapper>();
			NodeWrapper iter = this;
			while (iter.hasParent()) {
				wayToTarget.add(iter);
				iter = iter.getParent();
			}
			wayToTarget.add(iter);       //pøidání poèátku
			return wayToTarget;
		}

		int getNodeId() {
			return 0;
		}

		double getTotalCost() {
			return totalCost;
		}

	protected:

		NodeWrapper* parent;
		App::Node* node;
		double fromStart;
		double fromParent;
		double heuristicToEnd;
		double totalCost;



	};

}