#pragma once
#include "Path.h"

namespace App
{

	class EmptyPath : public App::Path
	{
	public:
		EmptyPath(shared_ptr<GoalInterface> goal);
		virtual ~EmptyPath();
		virtual vector<shared_ptr<Node>> getNodes() const override;
		virtual int getSize() const override;
		virtual void addToEnd(shared_ptr<Node> node) override;
		virtual shared_ptr<Node> get(int i) override;
		virtual bool hasNext(shared_ptr<Node> node) override;
		virtual shared_ptr<Node> getNext(shared_ptr<Node> node) override;
		virtual bool isFirstCloserOrSameToEnd(shared_ptr<Node> first, shared_ptr<Node> second) override;
		virtual int getIndex(shared_ptr<Node> node) override;
		virtual shared_ptr<GoalInterface> getGoal() const override;

	};

}
