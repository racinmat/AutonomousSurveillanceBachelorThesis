#include "PathHandler.h"

namespace App
{

	PathHandler::PathHandler()
	{
	}


	PathHandler::~PathHandler()
	{
	}

	vector<shared_ptr<State>> PathHandler::getPath(shared_ptr<State> end)
	{
		vector<shared_ptr<State>> path = vector<shared_ptr<State>>();
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

	vector<shared_ptr<State>> PathHandler::getPath(shared_ptr<State> start, shared_ptr<State> end)
	{
		vector<shared_ptr<State>> path = vector<shared_ptr<State>>();
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

}