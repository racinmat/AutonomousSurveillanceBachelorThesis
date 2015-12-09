#include "PathHandler.h"

namespace App
{

	PathHandler::PathHandler(shared_ptr<CollisionDetector> collisionDetector) : 
		collisionDetector(collisionDetector)
	{
	}


	PathHandler::~PathHandler()
	{
	}

	vector<shared_ptr<LinkedState>> PathHandler::getPath(shared_ptr<LinkedState> end)
	{
		vector<shared_ptr<LinkedState>> path = vector<shared_ptr<LinkedState>>();
		auto iterNode = end;
		do
		{
			path.push_back(iterNode);
			iterNode = iterNode->getPrevious();
		} while (iterNode->getPrevious());

		path.push_back(iterNode);

		reverse(path.begin(), path.end());	//abych mìl cestu od zaèátku do konce
		return path;
	}

	vector<shared_ptr<State>> PathHandler::createStatePath(vector<shared_ptr<LinkedState>> path)
	{
		vector<shared_ptr<State>> newPath = vector<shared_ptr<State>>(path.size());
		for (size_t i = 0; i < path.size(); i++)
		{
			newPath[i] = make_shared<State>(*path[i].get());
		}

		return newPath;
	}

	vector<shared_ptr<LinkedState>> PathHandler::getPath(shared_ptr<LinkedState> start, shared_ptr<LinkedState> end)
	{
		vector<shared_ptr<LinkedState>> path = vector<shared_ptr<LinkedState>>();
		auto iterNode = end;
		do
		{
			path.push_back(iterNode);
			iterNode = iterNode->getPrevious();
		} while (*iterNode.get() != *start.get());

		path.push_back(iterNode);

		reverse(path.begin(), path.end());	//abych mìl cestu od zaèátku do konce
		return path;
	}

	//narovná všechny trajektorie pøedtím, než se sputí optimalizace Dubinsem
	void PathHandler::straightenCrossingTrajectories(vector<shared_ptr<State>> path)
	{
		auto start = path[0];
		for (size_t i = 1; i < path.size(); i++)
		{
			auto end = path[i];
			bool intersecting = collisionDetector->areTrajectoriesIntersecting(start, end);
			while (intersecting)
			{
				auto uavs = collisionDetector->getIntersectingUavs(start, end);
				//swap intersecting uavs in all states after end (including end)
				for (size_t j = i; j < path.size(); j++)
				{
					auto toBeSwapped = path[j];
					toBeSwapped->swapUavs(uavs.first, uavs.second);
				}
				intersecting = collisionDetector->areTrajectoriesIntersecting(start, end);
			}
		}
	}

}