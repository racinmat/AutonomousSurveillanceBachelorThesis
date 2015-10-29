#include "Path.h"

namespace App
{

	Path::Path()
	{
		nodes = std::vector<Node*>();
	}

	std::vector<Node*> Path::getNodes() const
	{
		return nodes;
	}

	void Path::addNode(Node* node)
	{
		nodes.push_back(node);
	}

}