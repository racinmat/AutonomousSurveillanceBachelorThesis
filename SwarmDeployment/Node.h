#pragma once
#include "Point.h"
#include <array>
#include "Map.h"
#include <memory>

namespace App
{

	class Node
	{
	public:
		Node(Point* point, Grid gridType, double cost);
		Node(Point* point, Grid gridType);
		virtual ~Node();
		Point* getPoint() const;
		double getCost() const;
		std::vector<std::shared_ptr<Node>> getNeighbors() const;
		void addNeighbor(std::shared_ptr<Node> node, int position);
		void increaseCost(double increase);
		Grid getGridType() const;
		bool contains(int x, int y, int distance);
		bool operator<(const Node& another); //needed for comparation when using count method of std::set
		bool operator>(const Node& another); //If nodes are same, wrappers should also behave as same.
		//I suppose count method uses <, > operators, it is written here: http://stackoverflow.com/questions/8118148/how-does-count-in-c-stl-set-work
	protected:
		Point* point;
		double cost;
		std::vector<std::shared_ptr<Node>> neighbors;	//vector, because each node has different amount of neighbors and array with size 8 returns some null values, when node has less neighbors than 8
		Grid gridType;
	};

}