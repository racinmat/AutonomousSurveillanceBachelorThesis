#pragma once
#include "Node.h"
#include <set>


namespace AStar
{

	class NodeWrapper
	{
	public:
		virtual ~NodeWrapper();
		NodeWrapper(NodeWrapper* parent, App::Node* node, App::Node* endNode);
		double getDistance(NodeWrapper* node);
		double getDistance(App::Node* node);
		std::set<NodeWrapper*> expand(App::Node* endNode);
		void recalculateHeuristic(App::Node* endNode);
		NodeWrapper* getParent() const;
		App::Node* getNode() const;
		double getX() const;
		double getY() const;
		double getFromStart() const;
		double getHeuristicToEnd() const;
		bool hasParent();
		std::set<NodeWrapper*> getWay();
		double getTotalCost();
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