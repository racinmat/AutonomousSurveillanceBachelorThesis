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
		for (size_t i = 1; i < path.size(); i++)
		{
			for (auto uav : path[i]->getUavs())
			{
				auto start = path[i - 1]->getUav(uav);
				auto end = uav;
				for (size_t j = 1; j < path.size(); j++)
				{
					for (auto another : path[j]->getUavs())
					{
						auto anotherStart = path[j - 1]->getUav(another);
						auto anotherEnd = another;

						bool intersecting = collisionDetector->areLineSegmentsIntersecting(start, end, anotherStart, anotherEnd);
						//swap intersecting uavs in all states after end (including end)
						if (intersecting)
						{
							//jedna cesta je vždy delší než jiná, protože ošetøení na kolize mezi sousedními stavy je již v rrt-path
							vector<shared_ptr<PointParticle>> pathToEnd1;	//cesty køížících se uav do konce. cesty se zkopírují sem, pak se vloží èekací stavy a pak se odzadu vloží cesty odsud do pùvodních cest
							vector<shared_ptr<PointParticle>> pathToEnd2;

							for (size_t k = i; k < path.size(); k++)
							{
								pathToEnd1.push_back(path[k]->getUav(uav)->getPointParticle());
							}

							for (size_t k = j; k < path.size(); k++)
							{
								pathToEnd2.push_back(path[k]->getUav(another)->getPointParticle());
							}

							//nyní vložím èekací stavy, abych mohl prohodit cesty
							int waitingStatesCount = abs(int(i - j));
							vector<shared_ptr<State>> waitingStates;
							int startIndex = min(i, j);	//aby se zaèala prohazovat cesta od prvku s kolizemi, který je nejblíže cíli

							auto waitingState = path[startIndex - 1];
							for (size_t k = 0; k < waitingStatesCount; k++)
							{
								waitingStates.push_back(make_shared<State>(*waitingState.get()));
							}
							path.insert(path.begin() + startIndex - 1, waitingStates.begin(), waitingStates.end());


							//odzadu zapisuji do pole prohozené cesty
							{
								int index = pathToEnd2.size() - 1;
								for (size_t k = path.size() - 1; k >= i; k++)
								{
									path[k]->getUav(uav)->setPointParticle(pathToEnd2[index]);
									index--;
								}
							}
							{
								int index = pathToEnd1.size() - 1;
								for (size_t k = path.size() - 1; k >= j; k++)
								{
									path[k]->getUav(another)->setPointParticle(pathToEnd1[index]);
									index--;
								}

							}

							//místo výmìny id vymìním èásti cest, protože budu mìnit rùznì dlouhé úseky (rùznì dlouhé na poèet stavù)
//							for (size_t k = startIndex; k < path.size(); k++)
//							{
//								auto toBeSwapped = path[k];
//								toBeSwapped->swapUavs(uav, another);
//							}
						}
					}
				}

			}
		}
	}

}