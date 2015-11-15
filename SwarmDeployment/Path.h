#pragma once
#include <vector>
#include "Node.h"
#include <list>

namespace App
{

	class Path
	{
	public:
		Path();
		virtual ~Path();
		std::vector<std::shared_ptr<Node>> getNodes() const;
		int getSize() const;
		void reverse();	//kvůli kompatibilitě s Petrlíkem.
		//todo: metodu reverse odstranit hned, jak to bude možné
		void addToEnd(std::shared_ptr<Node> node);
		shared_ptr<Node> get(int i);
	protected:
		std::vector<std::shared_ptr<Node>> nodes;
	};

}