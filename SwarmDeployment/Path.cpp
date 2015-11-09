#include "Path.h"

namespace App
{

	Path::Path()
	{
		nodes = std::list<std::shared_ptr<Node>>();
	}

	Path::~Path()
	{
	}

	std::list<std::shared_ptr<Node>> Path::getNodes() const
	{
		return nodes;
	}

	void Path::addToStart(std::shared_ptr<Node> node)
	{
		nodes.push_front(node);
	}

	void Path::addToEnd(std::shared_ptr<Node> node)
	{
		nodes.push_back(node);
	}

}