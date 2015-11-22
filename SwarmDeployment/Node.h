#pragma once
#include "Point.h"
#include "Map.h"
#include <memory>
#include "Enums.h"

using namespace std;

namespace App
{

	class Node
	{
	public:
		Node(shared_ptr<Point> point, Grid gridType, double cost);
		Node(shared_ptr<Point> point, Grid gridType);
		virtual ~Node();
		shared_ptr<Point> getPoint() const;
		double getCost() const;
		std::vector<std::shared_ptr<Node>> getNeighbors() const;
		void addNeighbor(std::shared_ptr<Node> node, int position);
		void increaseCost(double increase);
		Grid getGridType() const;
		bool contains(int x, int y, int distance);
		friend bool operator==(const Node& lhs, const Node& rhs);
		friend bool operator!=(const Node& lhs, const Node& rhs);
		friend std::size_t hash_value(const Node& obj);

	protected:
		shared_ptr<Point> point;
		double cost;
		Grid gridType;
		std::vector<std::shared_ptr<Node>> neighbors;	//vector, because each node has different amount of neighbors and array with size 8 returns some null values, when node has less neighbors than 8
	};

}