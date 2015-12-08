#include "PathOptimizer.h"
#include "Random.h"
#include "Dubins/geom/geom.h"
#include "Configuration.h"
#include "PathHandler.h"

using namespace geom;

namespace App
{


	PathOptimizer::PathOptimizer(shared_ptr<DistanceResolver> distanceResolver, shared_ptr<Configuration> configuration, 
		shared_ptr<CarLikeMotionModel> motionModel, shared_ptr<CollisionDetector> collisionDetector, 
		shared_ptr<LoggerInterface> logger) :
		distanceResolver(distanceResolver), configuration(configuration), motionModel(motionModel), collisionDetector(collisionDetector), 
		logger(logger)
	{
	}


	PathOptimizer::~PathOptimizer()
	{
	}

	//optimalizuje cestu pomocí Dubinsových manévrù
	vector<shared_ptr<State>> PathOptimizer::optimizePath(vector<shared_ptr<State>> path, shared_ptr<Map> map)
	{
		double pathLength = distanceResolver->getLengthOfPath(path);

		shared_ptr<LinkedState> endOfPath = path[path.size() - 1]; //úplnì poslední prvek celé cesty, cíl
		int stopLimit = 100;	//kolikrát za sebou se nesmí aplikování Dubinse zlepšit trajektorie, aby se algoritmus zastavil
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
			auto start = path[startIndex];
			auto endOriginal = path[endIndex];

			auto end = make_shared<LinkedState>(*endOriginal.get());	//kvùli prohazování køížejících se UAV vytvoøím kopii, kterou modifikuji. Až když zjistím, že je nová trajektorie v poøádku, uložím to do pùvodního stavu

			auto pair = optimizePathBetween(start, end, map);
			bool isPathChanged = pair.second;
			auto trajectoryPart = pair.first;

			if (isPathChanged)
			{
				trajectoryPart[0]->setPrevious(start);	//navazuji zaèátek nové trajektorie na pøedchoz èást trasy
				endOfPath = trajectoryPart[trajectoryPart.size() - 1];
				bool isEndOfPath = *endOriginal.get() == *endOfPath.get();	//zda je end koncem celé cesty
				if (isEndOfPath)
				{
					endOfPath = end;	//poslední prvek trajectoryPart
				}
				else
				{
					shared_ptr<LinkedState> afterEnd = path[endIndex + 1];
					afterEnd->setPrevious(end);

					//pøepoèítání èasù na úseku za optimalizovanou èástí
					double previousTime = end->getTime();
					for (size_t i = endIndex + 1; i < path.size(); i++)
					{
						previousTime += configuration->getEndTime();
						path[i]->setTime(previousTime);
					}
				}
				path = PathHandler::getPath(endOfPath);
				if (distanceResolver->getLengthOfPath(path) < pathLength)
				{
					notImprovedCount = 0;
					pathLength = distanceResolver->getLengthOfPath(path);
				} else
				{
					notImprovedCount++;
				}
			} else
			{
				notImprovedCount++;
			}
		}
		return PathHandler::getPath(endOfPath);
	}

	//bool øíká, zda se cesta zmìnila
	pair<vector<shared_ptr<State>>, bool> PathOptimizer::optimizePathBetween(shared_ptr<State> start, shared_ptr<State> end, shared_ptr<Map> map)
	{
		straightenCrossingTrajectories(start, end);	//pokud se køíží trajektorie, pak nemohu optimalizovat

		double maxSpeed = configuration->getDistanceOfNewNodes();	//také reprezentuje poèet pixelù, které v car like modelu urazí uav za sekundu
		//pøedpoèítám si délky všech dubinsù pøedem
		unordered_map<Uav, pair<Dubins, bool>, UavHasher> dubinsTrajectories = unordered_map<Uav, pair<Dubins, bool>, UavHasher>();	//øíká, zda je dubins kratší než pùvodní trajektorie nebo ne
		bool areAllDubinsTrajectoriesLonger = true;
		for (auto uav : end->getUavs())
		{
			double length = distanceResolver->getLengthOfPath(start, end, uav);

			auto dubins = Dubins(start->getUav(uav)->getPointParticle()->toPosition(), uav->getPointParticle()->toPosition(), motionModel->getMinimalCurveRadius());
			double newLength = dubins.getLength();	//vrací délku celého manévru
			bool isDubinsShorter = newLength < length;
			dubinsTrajectories[*uav.get()] = make_pair(dubins, isDubinsShorter);
			areAllDubinsTrajectoriesLonger = areAllDubinsTrajectoriesLonger && !isDubinsShorter;
		}

		logger->logDubinsPaths(dubinsTrajectories);

		if (areAllDubinsTrajectoriesLonger)
		{
			return make_pair(PathHandler::getPath(start, end), false); // pùvodní cesta
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
			int stepCount = totalTime / configuration->getEndTime();	//poèet krokù, doba celého manévru / doba jednoho kroku

			if (isDubinsShorter && stepCount > largestStepCount)
			{
				largestStepCount = stepCount;
			}
		}

		if (largestStepCount == 0)	//tak malá cesta, že je menší než krok simulace
		{
			return make_pair(PathHandler::getPath(start, end), false); // pùvodní cesta
		}

		//zde provedu diskretizaci postupnì pro všechna uav najednou, kus po kusu a pøitom budu kontrolovat všechny podmínky
		int stepCount = largestStepCount;
		vector<shared_ptr<LinkedState>> newTrajectory = vector<shared_ptr<LinkedState>>(stepCount);
		auto previousState = start;
		for (size_t i = 0; i < stepCount; i++)
		{
			auto newState = make_shared<LinkedState>(*previousState.get());
			newState->setPrevious(previousState);
			newState->incrementTime(configuration->getEndTime());
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
					if (distanceCompleted + configuration->getDistanceOfNewNodes() < dubins.getLength())
					{
						auto newPosition = dubins.getPosition(distanceCompleted + configuration->getDistanceOfNewNodes());

						uav->getPointParticle()->getLocation()->setX(newPosition.getPoint().getX());
						uav->getPointParticle()->getLocation()->setY(newPosition.getPoint().getY());
						uav->getPointParticle()->getRotation()->setZ(newPosition.getAngle());
					}
					else
					{	//uav, které už dorazilo do cíle, "poèká" na ostatní
						uav->getPointParticle()->setLocation(end->getUav(uav)->getPointParticle()->getLocation());
						uav->getPointParticle()->setRotation(end->getUav(uav)->getPointParticle()->getRotation());
					}
				} else
				{
					shared_ptr<LinkedState> currentOldState = end;
					while (currentOldState->getTime() > newState->getTime())
					{
						currentOldState = currentOldState->getPrevious();
					}
					//pokud je kratší pùvodní cesta pro dané uav, bere se pro dané uav pùvodní cesta
					uav->getPointParticle()->setLocation(currentOldState->getUav(uav)->getPointParticle()->getLocation());
					uav->getPointParticle()->setRotation(currentOldState->getUav(uav)->getPointParticle()->getRotation());
				}

			}

			//validace
			if (!collisionDetector->isStateValid(start, end, map))
			{
				return make_pair(PathHandler::getPath(start, end), false); // pùvodní cesta
			}

			previousState = newState;
			newTrajectory[i] = newState;
//			logger->logNewState(previousState, newState, true);
		}

		auto lastState = newTrajectory[newTrajectory.size() - 1];
		end->setPrevious(lastState);
		end->setTime(lastState->getTime() + configuration->getEndTime());
		newTrajectory.push_back(end);

		double newLength = distanceResolver->getLengthOfPath(newTrajectory[0], end);

		return make_pair(newTrajectory, true);
	}

	void PathOptimizer::setLogger(const shared_ptr<LoggerInterface> logger_interface)
	{
		logger = logger_interface;
	}

	void PathOptimizer::straightenCrossingTrajectories(shared_ptr<LinkedState> start, shared_ptr<LinkedState> end)
	{
		bool intersecting = collisionDetector->areTrajectoriesIntersecting(start, end);
		while (intersecting)
		{
			auto uavs = collisionDetector->getIntersectingUavs(start, end);
			//swap intersecting uavs
			end->swapUavs(uavs.first, uavs.second);
			intersecting = collisionDetector->areTrajectoriesIntersecting(start, end);
		}
	}

}
