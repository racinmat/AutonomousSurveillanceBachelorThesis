#pragma once
#include "PointParticle.h"
#include <vector>
#include "Path.h"
#include "UavForRRT.h"

namespace App
{
	class UavGroup
	{
	public:
		UavGroup();
		virtual ~UavGroup();
		UavGroup(vector<shared_ptr<UavForRRT>> uavs, shared_ptr<Path> guiding_path);
		virtual vector<shared_ptr<UavForRRT>> getUavs() const;
		virtual void setUavs(const vector<shared_ptr<UavForRRT>> uavs);
		virtual shared_ptr<Path> getGuidingPath() const;
		virtual void setGuidingPath(const shared_ptr<Path> guiding_path);
		virtual void addUav(shared_ptr<UavForRRT> uav);
		virtual shared_ptr<Node> getBestNode();

	protected:
		vector<shared_ptr<UavForRRT>> uavs;
		shared_ptr<Path> guidingPath;
	};

}

