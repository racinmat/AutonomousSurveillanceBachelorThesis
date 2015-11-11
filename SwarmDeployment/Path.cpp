#include "Path.h"

namespace App
{

	Path::Path()
	{
		nodes = std::vector<std::shared_ptr<Node>>();
	}

	Path::~Path()
	{
	}

	std::vector<std::shared_ptr<Node>> Path::getNodes() const
	{
		return nodes;
	}

	int Path::getSize() const
	{
		return nodes.size();
	}

	shared_ptr<Node> Path::get(int i)
	{
		return nodes[i];
	}

	void Path::addToEnd(std::shared_ptr<Node> node)
	{
		nodes.push_back(node);
	}

}