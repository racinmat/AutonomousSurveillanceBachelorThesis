#pragma once
#include "PointParticle.h"
#include <vector>
#include "Path.h"

namespace App
{
	class UavGroup
	{
	public:
		UavGroup();
		virtual ~UavGroup();
		UavGroup(vector<shared_ptr<PointParticle>> uavs, shared_ptr<Path> guiding_path, vector<int> indexes);
		virtual vector<shared_ptr<PointParticle>> getUavs() const;
		virtual void setUavs(const vector<shared_ptr<PointParticle>> uavs);
		virtual shared_ptr<Path> getGuidingPath() const;
		virtual void setGuidingPath(const shared_ptr<Path> guiding_path);
		virtual void addUav(shared_ptr<PointParticle> uav, int index);
		virtual vector<int> getUavIndexes() const;
		int guidingPathIndex;	//todo: refactorovat

	protected:
		vector<shared_ptr<PointParticle>> uavs;
		shared_ptr<Path> guidingPath;
		vector<int> uavIndexes;	//todo: tohle zrefactorovat spoleènì s current_index
	};

}

