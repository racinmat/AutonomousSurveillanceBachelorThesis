#include "UavGroup.h"

namespace App
{

	UavGroup::UavGroup()
	{
	}


	UavGroup::~UavGroup()
	{
	}

	App::UavGroup::UavGroup(vector<shared_ptr<Uav>> uavs, shared_ptr<Path> guiding_path, vector<int> indexes) : uavs(uavs),
		guidingPath(guiding_path), uavIndexes(indexes)
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

	void UavGroup::addUav(shared_ptr<Uav> uav, int index)
	{
		uavs.push_back(uav);
		uavIndexes.push_back(index);
	}

	vector<int> UavGroup::getUavIndexes() const
	{
		return uavIndexes;
	}

	int UavGroup::getBestIndex() const
	{
		int bestIndex = 0;
		for (auto uav : uavs)
		{
			if (uav->current_index[guidingPathIndex] > bestIndex)
			{
				bestIndex = uav->current_index[guidingPathIndex];
			}
		}
		return bestIndex;
	}
}
