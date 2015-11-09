#pragma once
#include "Node.h"
#include <set>
#include <memory>


namespace AStar
{

	class NodeWrapper : public std::enable_shared_from_this<NodeWrapper>
	{
	public:
		virtual ~NodeWrapper();
		NodeWrapper(std::shared_ptr<NodeWrapper> parent, std::shared_ptr<App::Node> node, std::shared_ptr<App::Node> endNode);
		virtual double getDistance(std::shared_ptr<NodeWrapper> node);
		virtual double getDistance(std::shared_ptr<App::Node> node);
		virtual std::set<std::shared_ptr<NodeWrapper>> expand(std::shared_ptr<App::Node> endNode);
		virtual void recalculateHeuristic(std::shared_ptr<App::Node> endNode);
		virtual std::shared_ptr<NodeWrapper> getParent() const;
		virtual std::shared_ptr<App::Node> getNode() const;
		virtual double getX() const;
		virtual double getY() const;
		virtual double getFromStart() const;
		virtual double getHeuristicToEnd() const;
		virtual bool hasParent();
		virtual std::vector<std::shared_ptr<NodeWrapper>> getWay();
		virtual double getTotalCost();
		virtual bool operator==(const NodeWrapper& another);
		virtual bool operator==(const App::Node& another);
		virtual bool operator!=(const NodeWrapper& another);
		virtual bool operator!=(const App::Node& another);
		virtual std::shared_ptr<NodeWrapper> getPointer();
	protected:
		std::string coords;
		std::shared_ptr<NodeWrapper> parent;
		std::shared_ptr<App::Node> node;
		double fromStart;
		double fromParent;
		double heuristicToEnd;
		double totalCost;
		int pathLength; //použito pro urèení délky hledané cesty. Pro alokaci pole, kam se uloží nalezená cesta.
		static int lastId;
		int id;
	};

}