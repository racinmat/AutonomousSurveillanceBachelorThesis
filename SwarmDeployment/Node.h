#pragma once
#include "Point.h"
#include "Map.h"
#include <memory>
#include "Enums.h"
#include <tuple>

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
		vector<shared_ptr<Node>> getNeighbors() const;
		vector<shared_ptr<Node>> getNeighborsWithoutObstacles() const;
		vector<shared_ptr<Node>> getDirectNeighbors() const;
		void addNeighbor(shared_ptr<Node> node, bool isDiagonal);
		void increaseCost(double increase);
		Grid getGridType() const;
		bool contains(int x, int y, int distance);
		bool contains(Point point, int distance);
		bool contains(shared_ptr<Point> point, int distance);
		friend bool operator==(const Node& lhs, const Node& rhs);
		friend bool operator!=(const Node& lhs, const Node& rhs);
		friend size_t hash_value(const Node& obj);
		virtual double getDistanceToObstacle() const;
		virtual void setDistanceToObstacle(const double distance_to_obstacle);

	protected:
		shared_ptr<Point> point;
		double cost;
		Grid gridType;
		//vector, because each node has different amount of neighbors and array with size 8 returns some null values, when node has less neighbors than 8
		vector<tuple<shared_ptr<Node>, bool>> neighbors;	//bool is diagonal. true idf neighbor is direct (only x XOR y changes, not both), true, if x AND y changes (is diagonal)
		double distanceToObstacle;	//distance to nearest obstacle
	};

}