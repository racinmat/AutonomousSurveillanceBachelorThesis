#pragma once
#include "Node.h"
#include <set>
#include <memory>

using namespace App;

namespace AStar
{

	class NodeWrapper : public enable_shared_from_this<NodeWrapper>
	{
	public:
		virtual ~NodeWrapper();
		NodeWrapper(shared_ptr<NodeWrapper> parent, shared_ptr<Node> node, shared_ptr<Node> endNode);
		virtual double getDistance(shared_ptr<NodeWrapper> node);
		virtual double getDistance(shared_ptr<Node> node);
		virtual set<shared_ptr<NodeWrapper>> expand(shared_ptr<Node> endNode);
		virtual void recalculateHeuristic(shared_ptr<Node> endNode);
		virtual shared_ptr<NodeWrapper> getParent() const;
		virtual shared_ptr<Node> getNode() const;
		virtual double getX() const;
		virtual double getY() const;
		virtual double getFromStart() const;
		virtual double getHeuristicToEnd() const;
		virtual bool hasParent();
		virtual vector<shared_ptr<NodeWrapper>> getWay();
		virtual double getTotalCost();
		virtual bool operator==(const NodeWrapper& another);
		virtual bool operator==(const Node& another);
		virtual bool operator!=(const NodeWrapper& another);
		virtual bool operator!=(const Node& another);
	protected:
		string coords;
		shared_ptr<NodeWrapper> parent;
		shared_ptr<Node> node;
		double fromStart;
		double fromParent;
		double heuristicToEnd;
		double totalCost;
		int pathLength; //použito pro urèení délky hledané cesty. Pro alokaci pole, kam se uloží nalezená cesta.
	};

}