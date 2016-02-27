#include "PathOptimizer.h"
#include "Random.h"
#include "Dubins/geom/geom.h"
#include "Configuration.h"
#include "PathHandler.h"

using namespace geom;

namespace App
{


	PathOptimizer::PathOptimizer(shared_ptr<DistanceResolver> distanceResolver, shared_ptr<Configuration> configuration, 
		shared_ptr<MotionModel> motionModel, shared_ptr<CollisionDetector> collisionDetector, 
		shared_ptr<LoggerInterface> logger) :
		distanceResolver(distanceResolver), configuration(configuration), motionModel(motionModel), collisionDetector(collisionDetector), 
		logger(logger)
	{
	}


	PathOptimizer::~PathOptimizer()
	{
	}

	//optimalizuje cestu pomocí Dubinsových manévrù
	vector<shared_ptr<State>> PathOptimizer::optimizePathByDubins(vector<shared_ptr<State>> path, shared_ptr<Map> map)
	{
		double pathLength = distanceResolver->getLengthOfPath(path);

		shared_ptr<State> endOfPath = path[path.size() - 1]; //úplnì poslední prvek celé cesty, cíl
		int stopLimit = 500;	//kolikrát za sebou se nesmí aplikování Dubinse zlepšit trajektorie, aby se algoritmus zastavil
		int notImprovedCount = 0;

		while (notImprovedCount < stopLimit)
		{
			auto startIndex = Random::index(path);	//indexy jsou kvùli jednoduššímu pohybu v poli
			auto endIndex = Random::index(path);
			while (startIndex == endIndex)
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
			auto trajectoryPart = pair.first;
			bool isPathChanged = pair.second;

			if (isPathChanged)
			{
				vector<shared_ptr<State>> pathFirstPart;		//èást pøed dubinsem
				vector<shared_ptr<State>> pathMiddlePart = trajectoryPart;	//èást nahrazená dubinsem
				vector<shared_ptr<State>> pathLastPart;		//èást po dubinsovi
				
				if (startIndex > 0)	//pøed vyseknutou èástí je další èást
				{
					pathFirstPart = vector<shared_ptr<State>>(path.begin(), path.begin() + startIndex);	//subvector, vykousnutí èásti vektoru, prvek na pozici startIndex už tam není
				}

				if (endIndex < path.size() - 1)	//po vyseknuté èásti je ještì další èást
				{
					pathLastPart = vector<shared_ptr<State>>(path.begin() + endIndex + 1, path.end());	//subvector, vykousnutí èásti vektoru
				}

				//spojení 3 èástí cesty
				auto newPath = pathFirstPart;
				newPath.insert(newPath.end(), pathMiddlePart.begin(), pathMiddlePart.end());
				newPath.insert(newPath.end(), pathLastPart.begin(), pathLastPart.end());

				if (distanceResolver->getLengthOfPath(newPath) < pathLength)
				{
					notImprovedCount = 0;
					pathLength = distanceResolver->getLengthOfPath(newPath);
					path = newPath;
				} else
				{
					notImprovedCount++;
				}
			} else
			{
				notImprovedCount++;
			}
		}
		return path;
	}

	//bool øíká, zda se cesta zmìnila
	pair<vector<shared_ptr<State>>, bool> PathOptimizer::optimizePathPart(int startIndex, int endIndex, shared_ptr<Map> map, vector<shared_ptr<State>> path)
	{
		vector<shared_ptr<State>>::const_iterator startIterator = path.begin() + startIndex;	//iterátory použity pouze k vykousnutí èásti cesty, kterou budu mìnit
		vector<shared_ptr<State>>::const_iterator endIterator = path.begin() + endIndex + 1;
		vector<shared_ptr<State>> pathPart(startIterator, endIterator);
		auto start = pathPart[0];
		auto end = pathPart[pathPart.size() - 1];

		double maxSpeed = configuration->getDistanceOfNewNodes();	//také reprezentuje poèet pixelù, které v car like modelu urazí uav za sekundu
		//pøedpoèítám si délky všech dubinsù pøedem
		unordered_map<Uav, pair<Dubins, bool>, UavHasher> dubinsTrajectories = unordered_map<Uav, pair<Dubins, bool>, UavHasher>();	//øíká, zda je dubins kratší než pùvodní trajektorie nebo ne
		bool areAllDubinsTrajectoriesLonger = true;
		for (auto uav : end->getUavs())
		{
			double length = distanceResolver->getLengthOfPath(pathPart, uav);

			auto dubins = Dubins(start->getUav(uav)->getPointParticle()->toPosition(), uav->getPointParticle()->toPosition(), motionModel->getMinimalCurveRadius());
			double newLength = dubins.getLength();	//vrací délku celého manévru
			bool isDubinsShorter = newLength < length;
			dubinsTrajectories[*uav.get()] = make_pair(dubins, isDubinsShorter);
			areAllDubinsTrajectoriesLonger = areAllDubinsTrajectoriesLonger && !isDubinsShorter;
		}

		logger->logDubinsPaths(dubinsTrajectories);

		if (areAllDubinsTrajectoriesLonger)
		{
			return make_pair(pathPart, false); // pùvodní cesta
		}

		//nalezení nejdelšího dubbinse ze všech, které jsou kratší než pùvodní trajektorie, podle nìj se bude diskretizovat
		int largestStepCount = 0;
		for (auto uav : end->getUavs())
		{
			auto pair = dubinsTrajectories[*uav.get()];
			auto dubins = pair.first;
			auto isDubinsShorter = pair.second;
			double newLength = dubins.getLength();	//vrací délku celého manévru
			double totalTime = newLength / maxSpeed;	//doba cesty
			int stepCount = totalTime / configuration->getTimeStep();	//poèet krokù, doba celého manévru / doba jednoho kroku

			if (isDubinsShorter && stepCount > largestStepCount)
			{
				largestStepCount = stepCount;
			}
		}

		if (largestStepCount == 0)	//tak malá cesta, že je menší než krok simulace
		{
			return make_pair(pathPart, false); // pùvodní cesta
		}

		//zde provedu diskretizaci postupnì pro všechna uav najednou, kus po kusu a pøitom budu kontrolovat všechny podmínky
		int stepCount = largestStepCount;
		vector<shared_ptr<State>> newTrajectory = vector<shared_ptr<State>>(stepCount + 1);
		auto previousState = start;
		for (size_t i = 0; i <= stepCount; i++)
		{
			auto newState = make_shared<State>(*previousState.get());
			double distanceCompleted = i * configuration->getDistanceOfNewNodes();	//uražená cesta v dubinsovì manévru

			unordered_map<Uav, shared_ptr<CarLikeControl>, UavHasher> inputs = unordered_map<Uav, shared_ptr<CarLikeControl>, UavHasher>();
			//zde se pro každé UAV vybere podle typu Dubinsova manévru vhodný input pro motion model
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

						//TODO: zde uložit podle pozice v Dubinsovì manévru vstup
						switch (dubins.getCurrentManeuver(distanceCompleted))
						{
						case ManeuverPart::L:
							uav->setPreviousInput(make_shared<CarLikeControl>(configuration->getDistanceOfNewNodes(), configuration->getMaxTurn()));
							break;
						case ManeuverPart::S:
							uav->setPreviousInput(make_shared<CarLikeControl>(configuration->getDistanceOfNewNodes(), 0));
							break;
						case ManeuverPart::R: 
							uav->setPreviousInput(make_shared<CarLikeControl>(configuration->getDistanceOfNewNodes(), - configuration->getMaxTurn()));
							break;
						}

						auto newPosition = dubins.getPosition(distanceCompleted);

						pointParticle->getLocation()->setX(newPosition.getPoint().getX());
						pointParticle->getLocation()->setY(newPosition.getPoint().getY());
						pointParticle->getRotation()->setZ(newPosition.getAngle());
					}
					else
					{	//uav, které už dorazilo do cíle, "poèká" na ostatní
						pointParticle->setLocation(end->getUav(uav)->getPointParticle()->getLocation());
						pointParticle->setRotation(end->getUav(uav)->getPointParticle()->getRotation());
					}
				} else
				{
					//pokud je kratší pùvodní cesta pro dané uav, bere se pro dané uav pùvodní cesta, pokud je pùvodní cesta kratší na poèet krokù, uav "èeká" v cíli
					shared_ptr<State> currentOldState;
					if (i >= pathPart.size())
					{
						currentOldState = pathPart[pathPart.size() - 1];
					} else
					{
						currentOldState = pathPart[i];
					}
					uav->getPointParticle()->setLocation(currentOldState->getUav(uav)->getPointParticle()->getLocation());
					uav->getPointParticle()->setRotation(currentOldState->getUav(uav)->getPointParticle()->getRotation());
				}

			}

			//validace
			if (!collisionDetector->isStateValid(previousState, newState, map))
			{
				return make_pair(pathPart, false); // pùvodní cesta
			}

			previousState = newState;
			newTrajectory[i] = newState;
//			logger->logNewState(previousState, newState, true);
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
			//pøeskoèení prvního
			if (first)
			{
				first = false;
				previous = state;
				shorterPath.push_back(state);
				continue;
			}

			//porovnání stejných vzdálenosti sousedních stavù
			bool areStatesSame = true;
			for (auto uav : state->getUavs())
			{
				auto loc = uav->getPointParticle()->getLocation();
				auto previousLoc = previous->getUav(uav)->getPointParticle()->getLocation();
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
