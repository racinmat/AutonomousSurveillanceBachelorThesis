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
		std::vector<NodeWrapper*> getWay();
		double getTotalCost();
		bool operator==(const NodeWrapper& another);
		bool operator!=(const NodeWrapper& another);

	protected:
		std::string coords;
		NodeWrapper* parent;
		App::Node* node;
		double fromStart;
		double fromParent;
		double heuristicToEnd;
		double totalCost;
		int pathLength; //použito pro urèení délky hledané cesty. Pro alokaci pole, kam se uloží nalezená cesta.
	};

}