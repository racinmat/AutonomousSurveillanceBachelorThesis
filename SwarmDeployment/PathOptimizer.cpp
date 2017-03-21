#include "PathOptimizer.h"
#include "Random.h"
#include "Dubins/geom/geom.h"
#include "Configuration.h"
#include "PathHandler.h"
#include "Strings.h"

//kv�li na��t�n� vyu�it� pam�ti
//http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
#include "windows.h"
#include "psapi.h"

using namespace geom;

namespace App
{


	PathOptimizer::PathOptimizer(shared_ptr<DistanceResolver> distanceResolver, shared_ptr<Configuration> configuration, 
		shared_ptr<MotionModel> motionModel, shared_ptr<CollisionDetector> collisionDetector, 
		shared_ptr<LoggerInterface> logger, shared_ptr<Persister> persister, shared_ptr<Resampler> resampler) :
		distanceResolver(distanceResolver), configuration(configuration), motionModel(motionModel), collisionDetector(collisionDetector), 
		logger(logger), persister(persister), resampler(resampler)
	{
	}


	PathOptimizer::~PathOptimizer()
	{
	}

	//optimalizuje cestu pomoc� Dubinsov�ch man�vr�
	vector<shared_ptr<State>> PathOptimizer::optimizePathByDubins(vector<shared_ptr<State>> path, shared_ptr<Map> map)
	{
		double pathLength = distanceResolver->getLengthOfPath(path);
		int uavCount = path[0]->getUavs().size();
		int minPathPartDistance = 10;

		std::map<double, double> dataToGraph;

		shared_ptr<State> endOfPath = path[path.size() - 1]; //�pln� posledn� prvek cel� cesty, c�l
		int stopLimit;			//kolikr�t za sebou se nesm� aplikov�n� Dubinse zlep�it trajektorie, aby se algoritmus zastavil, vypad� to, �e s v�ce UAV se mus� zv��it i konstanta

		bool fuckingHack = configuration->getPlacementMethod() == PlacementMethod::Chain && configuration->getUavCount() > 4;
		if (fuckingHack) {
			stopLimit = 40 * uavCount;
		} else
		{
			stopLimit = 200;
//			stopLimit = 15000;
		}

		double minOptimizationSpeed;	//nejmen�� pom�r mezi po�tem iterac� a zrychlen�m cesty. Nyn� je to 24% na 1000 iterac� na uav. P�i vy���m po�tu UAV je celkov� cesta del��, tak�e se m�n� pod�l. Tak�e d�l�m konstantu po�tem UAV.
		if (fuckingHack) {
			minOptimizationSpeed = 0.05 / double(1000);
		}
		else
		{
//			minOptimizationSpeed = 0.05 / double(1000);
			minOptimizationSpeed = 0.2 / double(1000);
			//			minOptimizationSpeed = 0.000001 / double(1000);
		}

		int notImprovedCount = 0;
		int iterationCount = 0;
		double initialPathDistance = distanceResolver->getLengthOfPath(path);
		double distanceDifference = 0;
		double newPathDistance;

		while (true)
		{
			auto startIndex = Random::index(path);	//indexy jsou kv�li jednodu���mu pohybu v poli
			auto endIndex = Random::index(path);
			while (abs(startIndex - endIndex) < minPathPartDistance)
			{
				endIndex = Random::index(path);
			}
			if (startIndex > endIndex)
			{
				//swap
				auto temp = endIndex;
				endIndex = startIndex;
				startIndex = temp;
			}

			auto pair = optimizePathPart(startIndex, endIndex, map, path);
			bool isPathChanged = pair.second;

			if (isPathChanged)
			{
				auto trajectoryPart = pair.first;
				vector<shared_ptr<State>> pathFirstPart;		//��st p�ed dubinsem
				vector<shared_ptr<State>> pathMiddlePart = trajectoryPart;	//��st nahrazen� dubinsem
				vector<shared_ptr<State>> pathLastPart;		//��st po dubinsovi
				
				if (startIndex > 0)	//p�ed vyseknutou ��st� je dal�� ��st
				{
					pathFirstPart = vector<shared_ptr<State>>(path.begin(), path.begin() + startIndex);	//subvector, vykousnut� ��sti vektoru, prvek na pozici startIndex u� tam nen�
				}

				if (endIndex < path.size() - 1)	//po vyseknut� ��sti je je�t� dal�� ��st
				{
					pathLastPart = vector<shared_ptr<State>>(path.begin() + endIndex + 1, path.end());	//subvector, vykousnut� ��sti vektoru
				}

				//spojen� 3 ��st� cesty
				auto newPath = pathFirstPart;
				newPath.insert(newPath.end(), pathMiddlePart.begin(), pathMiddlePart.end());
				newPath.insert(newPath.end(), pathLastPart.begin(), pathLastPart.end());

				newPathDistance = distanceResolver->getLengthOfPath(newPath);
				if (newPathDistance < pathLength)
				{
					notImprovedCount = 0;
					pathLength = newPathDistance;
					path = newPath;
				} else
				{
					notImprovedCount++;
				}
				distanceDifference = initialPathDistance - newPathDistance;
			} else
			{
				newPathDistance = distanceResolver->getLengthOfPath(path);
				notImprovedCount++;
			}

			iterationCount++;
			dataToGraph[iterationCount] = newPathDistance;

			//pr�b�n� odeb�r�m duplicitn� stavy, abych zrychlil optimalizaci
			if (iterationCount % 1000 == 0)
			{
				path = removeDuplicitStates(path);
			}

			double optimizationSpeed = (distanceDifference / initialPathDistance) / double(iterationCount);
			if (distanceDifference > 0 && iterationCount > (40 * uavCount) && optimizationSpeed < minOptimizationSpeed)	//znormovan� rozd�l vzd�lenost� vyd�l�m po�tem iterac�
			{
//				std::ofstream out("output/optimization.txt");
//				out << "optimization speed limit is too slow: distance difference: " + to_string(distanceDifference) + 
//					", initial path distance: " + to_string(initialPathDistance) + 
//					", iteration count: " + to_string(iterationCount) + 
//					", optimizationSpeed: " + to_string(optimizationSpeed) + 
//					", minimal optimization speed: " + to_string(minOptimizationSpeed);
//				out.close();
				break;					//pojistka proti p��li� pomal� optimalizaci
			}
			if (notImprovedCount > stopLimit)
			{
//				std::ofstream out("output/optimization.txt");
//				out << "optimization did not change distance for many cycles: iteration count: " + to_string(iterationCount) +
//					", not improved count: " + to_string(notImprovedCount);
//				out.close();
				break;					//pokud se mnohokr�t za sebou nezkr�t� cesta, ooptimalizace skon��
			}

//			todo: rewrite to something more multiplatform
//			if (iterationCount % 10 == 0)
//			{
//				PROCESS_MEMORY_COUNTERS_EX pmc;
//				GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
//				SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
//				SIZE_T usedMB = virtualMemUsedByMe / 1024 / 1024;
//				if (usedMB > 1900)
//				{
//					break;
//				}
//				//				cout << to_string(usedMB) << endl;
//				//end of getting used memory by this process
//			}

		}

		string frequencyString;
		if (ceilf(resampler->getNewFrequency()) == resampler->getNewFrequency())	//if new frequency is integer
		{
			frequencyString = to_string(int(resampler->getNewFrequency()));
		} else
		{
			frequencyString = to_string(resampler->getNewFrequency());
		}

		persister->writeGraphData(dataToGraph, "dubinsResult-" + Strings::currentDateTime()  + "-" + frequencyString + "Hz");

		return path;
	}

	//bool ��k�, zda se cesta zm�nila
	pair<vector<shared_ptr<State>>, bool> PathOptimizer::optimizePathPart(int startIndex, int endIndex, shared_ptr<Map> map, vector<shared_ptr<State>> path)
	{
		vector<shared_ptr<State>>::const_iterator startIterator = path.begin() + startIndex;	//iter�tory pou�ity pouze k vykousnut� ��sti cesty, kterou budu m�nit
		vector<shared_ptr<State>>::const_iterator endIterator = path.begin() + endIndex + 1;
		vector<shared_ptr<State>> pathPart(startIterator, endIterator);
		auto start = pathPart[0];
		auto end = pathPart[pathPart.size() - 1];

		double maxSpeed;	//reprezentuje po�et pixel�, kter� v car like modelu uraz� uav za sekundu, tedy step v CarLikeControlu
		//p�edpo��t�m si d�lky v�ech dubins� p�edem
		unordered_map<Uav, pair<Dubins, bool>, UavHasher> dubinsTrajectories = unordered_map<Uav, pair<Dubins, bool>, UavHasher>();	//��k�, zda je dubins krat�� ne� p�vodn� trajektorie nebo ne
		bool areAllDubinsTrajectoriesLonger = true;
		for (auto uav : end->getUavs())
		{
			double length = distanceResolver->getLengthOfPath(pathPart, uav);

			auto dubins = Dubins(start->getBaseUav(uav)->getPointParticle()->toPosition(), uav->getPointParticle()->toPosition(), motionModel->getMinimalCurveRadius());
			double newLength = dubins.getLength();	//vrac� d�lku cel�ho man�vru
			bool isDubinsShorter = newLength < length;
			dubinsTrajectories[*uav.get()] = make_pair(dubins, isDubinsShorter);
			areAllDubinsTrajectoriesLonger = areAllDubinsTrajectoriesLonger && !isDubinsShorter;
		}

//		logger->logDubinsPaths(dubinsTrajectories);

		if (areAllDubinsTrajectoriesLonger)
		{
			return make_pair(pathPart, false); // p�vodn� cesta
		}

		//nalezen� nejdel��ho dubbinse z cest pro v�echna UAV, kter� jsou krat�� ne� p�vodn� trajektorie, podle n�j se bude diskretizovat
		int largestStepCount = 0;
		for (auto uav : end->getUavs())
		{
			maxSpeed = uav->getPreviousInput().getStep();	//speed == timeStep == d�lka kroku

			auto pair = dubinsTrajectories[*uav.get()];
			auto dubins = pair.first;
			auto isDubinsShorter = pair.second;
			double newLength = dubins.getLength();	//vrac� d�lku cel�ho man�vru
			double totalTime = newLength / maxSpeed;	//doba cesty
			int stepCount = floor(totalTime);	//po�et krok�, doba cel�ho man�vru == d�lka cel�ho man�vru / d�lka kroku

			if (isDubinsShorter && stepCount > largestStepCount)
			{
				largestStepCount = stepCount;
			}
		}

		if (largestStepCount == 0)	//tak mal� cesta, �e je men�� ne� krok simulace
		{
			return make_pair(pathPart, false); // p�vodn� cesta
		}

		//zde provedu diskretizaci postupn� pro v�echna uav najednou, kus po kusu a p�itom budu kontrolovat v�echny podm�nky
		int stepCount = largestStepCount;
		vector<shared_ptr<State>> newTrajectory = vector<shared_ptr<State>>(stepCount + 1);
		auto previousState = start;
		for (size_t i = 0; i <= stepCount; i++)
		{
			auto newState = make_shared<State>(*previousState.get());
			double distanceCompleted = i * maxSpeed;	//ura�en� cesta v dubinsov� man�vru

			//zde se pro ka�d� UAV vybere podle typu Dubinsova man�vru vhodn� input pro motion model
			for (auto uav : newState->getUavs())
			{
				auto pair = dubinsTrajectories[*uav.get()];
				auto dubins = pair.first;
				auto isDubinsShorter = pair.second;
				if (isDubinsShorter)
				{
					auto pointParticle = uav->getPointParticle();
					if (distanceCompleted < dubins.getLength())
					{

						//TODO: zde ulo�it podle pozice v Dubinsov� man�vru vstup
						switch (dubins.getCurrentManeuver(distanceCompleted))
						{
						case ManeuverPart::L:
							uav->setPreviousInput(CarLikeControl(maxSpeed, configuration->getMaxTurn()));
							break;
						case ManeuverPart::S:
							uav->setPreviousInput(CarLikeControl(maxSpeed, 0));
							break;
						case ManeuverPart::R: 
							uav->setPreviousInput(CarLikeControl(maxSpeed, - configuration->getMaxTurn()));
							break;
						}

						auto newPosition = dubins.getPosition(distanceCompleted);

						pointParticle->getLocation()->setX(newPosition.getPoint().getX());
						pointParticle->getLocation()->setY(newPosition.getPoint().getY());
						pointParticle->getRotation()->setZ(newPosition.getAngle());
					}
					else
					{	
						//uav, kter� u� dorazilo do c�le, "po�k�" na ostatn�
						pointParticle->setLocation(end->getBaseUav(uav)->getPointParticle()->getLocation());
						pointParticle->setRotation(end->getBaseUav(uav)->getPointParticle()->getRotation());

						uav->setPreviousInput(end->getUav(uav)->getPreviousInput());
					}
				} else
				{
					//pokud je krat�� p�vodn� cesta pro dan� uav, bere se pro dan� uav p�vodn� cesta, pokud je p�vodn� cesta krat�� na po�et krok�, uav "�ek�" v c�li
					shared_ptr<State> currentOldState;
					if (i >= pathPart.size())
					{
						currentOldState = pathPart[pathPart.size() - 1];
					} else
					{
						currentOldState = pathPart[i];
					}
					uav->getPointParticle()->setLocation(currentOldState->getBaseUav(uav)->getPointParticle()->getLocation());
					uav->getPointParticle()->setRotation(currentOldState->getBaseUav(uav)->getPointParticle()->getRotation());

					uav->setPreviousInput(currentOldState->getUav(uav)->getPreviousInput());

					auto y = uav->getPointParticle()->getLocation()->getY();
				}

			}

			//validace
			if (!collisionDetector->isStateValid(previousState, newState, map))
			{
				return make_pair(pathPart, false); // p�vodn� cesta
			}

			previousState = newState;
			newTrajectory[i] = newState;
		}

		auto lastState = newTrajectory[newTrajectory.size() - 1];
		newTrajectory.push_back(end);

		return make_pair(newTrajectory, true);
	}

	void PathOptimizer::setLogger(const shared_ptr<LoggerInterface> logger_interface)
	{
		logger = logger_interface;
	}

	vector<shared_ptr<State>> PathOptimizer::removeDuplicitStates(vector<shared_ptr<State>> path)
	{
		vector<shared_ptr<State>> shorterPath = vector<shared_ptr<State>>();
		bool first = true;
		shared_ptr<State> previous;
		for (auto state : path)
		{
			//p�esko�en� prvn�ho
			if (first)
			{
				first = false;
				previous = state;
				shorterPath.push_back(state);
				continue;
			}

			//porovn�n� stejn�ch vzd�lenosti sousedn�ch stav�
			bool areStatesSame = true;
			for (auto uav : state->getBaseUavs())
			{
				auto loc = uav->getPointParticle()->getLocation();
				auto previousLoc = previous->getBaseUav(uav)->getPointParticle()->getLocation();
				if (loc->getDistance(previousLoc) > 0.01)
				{
					areStatesSame = false;
				}
			}

			if (!areStatesSame)
			{
				shorterPath.push_back(state);
			}

			previous = state;
		}
		return shorterPath;
	}
}
