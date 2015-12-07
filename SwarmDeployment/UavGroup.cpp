#include "UavGroup.h"

namespace App
{

	UavGroup::UavGroup()
	{
	}


	UavGroup::~UavGroup()
	{
	}

	App::UavGroup::UavGroup(vector<shared_ptr<Uav>> uavs, shared_ptr<Path> guiding_path) : 
		uavs(uavs), guidingPath(guiding_path)
	{
	}

	vector<shared_ptr<Uav>> UavGroup::getUavs() const
	{
		return uavs;
	}

	void UavGroup::setUavs(const vector<shared_ptr<Uav>> uavs)
	{
		this->uavs = uavs;
	}

	shared_ptr<Path> UavGroup::getGuidingPath() const
	{
		return guidingPath;
	}

	void UavGroup::setGuidingPath(const shared_ptr<Path> guiding_path)
	{
		guidingPath = guiding_path;
	}

	void UavGroup::addUav(shared_ptr<Uav> uav)
	{
		uavs.push_back(uav);
	}

	shared_ptr<Node> UavGroup::getBestNode()
	{
		int bestIndex = 0;
		for (auto uav : uavs)
		{
			if (guidingPath->getIndex(uav->getCurrentGuidingPathPositions()->get(guidingPath)) > bestIndex)		//todo: možná zrefactorovat, aby guidingPath dokázala øíct, jaká node je best
			{
				bestIndex = guidingPath->getIndex(uav->getCurrentGuidingPathPositions()->get(guidingPath));
			}
		}
		return guidingPath->get(bestIndex);
	}

}
