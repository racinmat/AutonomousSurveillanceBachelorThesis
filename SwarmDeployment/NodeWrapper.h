#pragma once
#include "Node.h"
#include <set>
#include <memory>


namespace AStar
{

	class NodeWrapper
	{
	public:
		virtual ~NodeWrapper();
		NodeWrapper(NodeWrapper* parent, std::shared_ptr<App::Node> node, std::shared_ptr<App::Node> endNode);
		double getDistance(NodeWrapper* node);
		double getDistance(std::shared_ptr<App::Node> node);
		std::set<std::shared_ptr<NodeWrapper>> expand(std::shared_ptr<App::Node> endNode);
		void recalculateHeuristic(std::shared_ptr<App::Node> endNode);
		std::shared_ptr<NodeWrapper> getParent() const;
		std::shared_ptr<App::Node> getNode() const;
		double getX() const;
		double getY() const;
		double getFromStart() const;
		double getHeuristicToEnd() const;
		bool hasParent();
		std::vector<std::shared_ptr<NodeWrapper>> getWay();
		double getTotalCost();
		bool operator==(const NodeWrapper& another);
		bool operator!=(const NodeWrapper& another);

	protected:
		std::string coords;
		std::shared_ptr<NodeWrapper> parent;
		std::shared_ptr<App::Node> node;
		double fromStart;
		double fromParent;
		double heuristicToEnd;
		double totalCost;
		int pathLength; //použito pro urèení délky hledané cesty. Pro alokaci pole, kam se uloží nalezená cesta.
	};

}