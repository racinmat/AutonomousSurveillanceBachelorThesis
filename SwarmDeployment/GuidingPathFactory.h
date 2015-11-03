#pragma once
#include "Path.h"
#include "Map.h"
#include "LoggerInterface.h"
#include <set>

namespace App
{

	class GuidingPathFactory
	{
	public:
		GuidingPathFactory(LoggerInterface* logger);
		virtual ~GuidingPathFactory();
		std::vector<App::Path*> createGuidingPaths(std::vector<Node*> nodes, Node* start, std::vector<Node*> ends);
	
	protected:
		LoggerInterface* logger;
		Path* findPath(std::vector<Node*> nodes, Node* start, Node* end);
		std::set<Node*> opened;
		std::set<Node*> closed;
		Node* examineNextNode(Node* current, Node* end);
	};

}
