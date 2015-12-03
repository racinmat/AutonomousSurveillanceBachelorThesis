#pragma once
#include <vector>
#include "Node.h"
#include <list>

namespace App
{

	class Path
	{
	public:
		Path(shared_ptr<GoalInterface> goal);
		virtual ~Path();
		vector<shared_ptr<Node>> getNodes() const;
		int getSize() const;
		void reverse();	//kvůli kompatibilitě s Petrlíkem.
		//todo: metodu reverse odstranit hned, jak to bude možné
		void addToEnd(shared_ptr<Node> node);
		shared_ptr<Node> get(int i);
		friend bool operator==(const Path& lhs, const Path& rhs);
		friend bool operator!=(const Path& lhs, const Path& rhs);
		bool hasNext(shared_ptr<Node> node);
		shared_ptr<Node> getNext(shared_ptr<Node> node);
		bool isFirstCloserOrSameToEnd(shared_ptr<Node> first, shared_ptr<Node> second);
		size_t hash() const;
		int getIndex(shared_ptr<Node> node);
		virtual shared_ptr<GoalInterface> getGoal() const;

	protected:
		vector<shared_ptr<Node>> nodes;
		shared_ptr<GoalInterface> goal;
	};


	class PathHasher
	{
	public:
		size_t operator() (Path const& key) const
		{
			return key.hash();
		}
	};

}