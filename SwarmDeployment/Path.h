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
		virtual vector<shared_ptr<Node>> getNodes() const;
		virtual int getSize() const;
		virtual void addToEnd(shared_ptr<Node> node);
		virtual shared_ptr<Node> get(int i);
		friend bool operator==(const Path& lhs, const Path& rhs);
		friend bool operator!=(const Path& lhs, const Path& rhs);
		virtual bool hasNext(shared_ptr<Node> node);
		virtual shared_ptr<Node> getNext(shared_ptr<Node> node);
		virtual bool isFirstCloserOrSameToEnd(shared_ptr<Node> first, shared_ptr<Node> second);
		virtual size_t hash() const;
		virtual int getIndex(shared_ptr<Node> node);
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