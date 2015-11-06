#include "Path.h"

namespace App
{

	Path::Path()
	{
		nodes = std::list<Node*>();
	}

	std::list<Node*> Path::getNodes() const
	{
		return nodes;
	}

	void Path::addToStart(App::Node* node)
	{
		nodes.push_front(node);
	}

	void Path::addToEnd(Node* node)
	{
		nodes.push_back(node);
	}

}