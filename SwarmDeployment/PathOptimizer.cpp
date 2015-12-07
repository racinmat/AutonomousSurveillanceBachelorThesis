#include "PathOptimizer.h"
#include "Random.h"
#include "Dubins/geom/geom.h"
#include "Configuration.h"

using namespace geom;

namespace App
{


	PathOptimizer::PathOptimizer(shared_ptr<DistanceResolver> distanceResolver, shared_ptr<Configuration> configuration, shared_ptr<CarLikeMotionModel> motionModel, shared_ptr<CollisionDetector> collisionDetector) :
		distanceResolver(distanceResolver), configuration(configuration), motionModel(motionModel), collisionDetector(collisionDetector)
	{
	}


	PathOptimizer::~PathOptimizer()
	{
	}

	//optimalizuje cestu pomocí Dubinsových manévrù
	vector<shared_ptr<State>> PathOptimizer::optimizePath(vector<shared_ptr<State>> path)
	{
		int stopLimit = 100;	//kolikrát za sebou se nesmí aplikování Dubinse zlepšit trajektorie, aby se algoritmus zastavil
		int notImprovedCount = 0;
		double maxSpeed = configuration->getDistanceOfNewNodes();	//také reprezentuje poèet pixelù, které v car like modelu urazí uav za sekundu

		while (notImprovedCount < stopLimit)
		{
			auto start = Random::element(path);
			auto endOriginal = Random::element(path);
			while (*start.get() == *endOriginal.get())
			{
				endOriginal = Random::element(path);
			}
			if (start->getIndex() > endOriginal->getIndex())
			{
				//swap
				auto temp = endOriginal;
				endOriginal = start;
				start = temp;
			}

			auto end = make_shared<State>(*endOriginal.get());	//kvùli prohazování køížejících se UAV vytvoøím kopii, kterou modifikuji. Až když zjistím, že je nová trajektorie v poøádku, uložím to do pùvodního stavu

			straightenCrossingTrajectories(start, end);	//pokud se køíží trajektorie, pak nemohu optimalizovat

			//pøedpoèítám si délky všech dubinsù pøedem
			unordered_map<Uav, pair<Dubins, bool>, UavHasher> dubinsTrajectories = unordered_map<Uav, pair<Dubins, bool>, UavHasher>();	//øíká, zda je dubins kratší než pùvodní trajektorie nebo ne
			bool areAllDubinsTrajectoriesLonger = true;
			for (auto uav : endOriginal->getUavs())
			{
				double length = 0;
				for (auto i = endOriginal; *i.get() != *start.get(); i = i->getPrevious())	//i jede od konce po prvek, jehož pøedchùdce je zaèátek
				{
					auto previous = i->getPrevious();
					length += distanceResolver->getDistance(previous, i);	//todo: vymyslet, jak najít vždálenost poèítanou po kružnici, a ne po výsledných pøímkách
				}

				auto dubins = Dubins(uav->getPointParticle()->toPosition(), start->getUav(uav)->getPointParticle()->toPosition(), motionModel->getMinimalCurveRadius());
				double newLength = dubins.getLength();	//vrací délku celého manévru
				bool isDubinsShorter = newLength < length;
				dubinsTrajectories[*uav.get()] = make_pair(dubins, isDubinsShorter);
				areAllDubinsTrajectoriesLonger = areAllDubinsTrajectoriesLonger && !isDubinsShorter;
			}

			if (areAllDubinsTrajectoriesLonger)
			{
				continue;
			}

			//nalezení nejdelšího dubbinse ze všech, které jsou kratší než pùvodní trajektorie, podle nìj se bude diskretizovat
			pair<Uav, int> largestStepCount = pair<Uav, int>();
			for (auto uav : end->getUavs())
			{

				auto pair = dubinsTrajectories[*uav.get()];
				auto dubins = pair.first;
				auto isDubinsShorter = pair.second;
				double newLength = dubins.getLength();	//vrací délku celého manévru

				int totalTime = newLength / maxSpeed;	//doba cesty
				int stepCount = totalTime / configuration->getEndTime();	//poèet krokù, doba celého manévru / doba jednoho kroku

				if (isDubinsShorter && stepCount > largestStepCount.second)
				{
					largestStepCount = make_pair(*uav.get(), stepCount);
				}
			}


			//zde provedu diskretizaci postupnì pro všechna uav najednou, kus po kusu a pøitom budu kontrolovat všechny podmínky
			int stepCount = largestStepCount.second;
			vector<shared_ptr<State>> newTrajectory = vector<shared_ptr<State>>(stepCount);
			auto previousState = start;
			for (size_t i = 0; i < stepCount; i++)
			{
				auto newState = make_shared<State>(*previousState.get());
				//todo: zmìnit hodnoty a nastavit previous
				newTrajectory[i] = newState;
			}
		}
		return path;
	}

	void PathOptimizer::straightenCrossingTrajectories(shared_ptr<State> start, shared_ptr<State> end)
	{
		bool intersecting = collisionDetector->areTrajectoriesIntersecting(start, end);
		while (intersecting)
		{
			auto uavs = collisionDetector->getIntersectingUavs(start, end);
			//swap intersecting uavs
			int tempId = end->getUav(uavs.first)->getId();
			end->getUav(uavs.first)->setId(end->getUav(uavs.second)->getId());
			end->getUav(uavs.second)->setId(tempId);
			intersecting = collisionDetector->areTrajectoriesIntersecting(start, end);
		}
	}

}
