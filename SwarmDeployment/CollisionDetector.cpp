#include "CollisionDetector.h"
#include "Configuration.h"
#include "VCollide/Triangle3D.h"
#include "VCollide/ColDetect.h"

namespace App
{


	CollisionDetector::CollisionDetector(shared_ptr<Configuration> configuration) : 
		configuration(configuration)
	{
	}


	CollisionDetector::~CollisionDetector()
	{
	}

	bool CollisionDetector::areTrajectoriesIntersecting(shared_ptr<StateInterface> start, shared_ptr<StateInterface> end)
	{
		for (auto uav1 : start->getUavs())
		{
			for (auto uav2 : start->getUavs())
			{
				if (*uav1.get() != *uav2.get() && areLineSegmentsIntersecting(
					start->getUav(uav1)->getPointParticle()->getLocation(),
					end->getUav(uav1)->getPointParticle()->getLocation(),
					start->getUav(uav2)->getPointParticle()->getLocation(),
					end->getUav(uav2)->getPointParticle()->getLocation()))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool CollisionDetector::areLineSegmentsIntersecting(shared_ptr<Point> p1, shared_ptr<Point> p2, shared_ptr<Point> p3, shared_ptr<Point> p4)
	{
		auto p = lineLineIntersection(p1, p2, p3, p4);

		if (isfinite(p->getX()) && isfinite(p->getY()))
		{
			if (LinePointIntersection(p, p1, p2) &&
				LinePointIntersection(p, p3, p4))
			{
				return true;
			}

		}
		return false;
	}

	bool CollisionDetector::areLineSegmentsIntersecting(shared_ptr<Uav> uav1start, shared_ptr<Uav> uav1end, shared_ptr<Uav> uav2start, shared_ptr<Uav> uav2end)
	{
		return areLineSegmentsIntersecting(
			uav1start->getPointParticle()->getLocation(), uav1end->getPointParticle()->getLocation(),
			uav2start->getPointParticle()->getLocation(), uav2end->getPointParticle()->getLocation());
	}

	bool CollisionDetector::LinePointIntersection(shared_ptr<Point> q, shared_ptr<Point> p1, shared_ptr<Point> p2)
	{
		double tolerance = 1e-10;

		double x = q->getX();
		double y = q->getY();
		double x1 = p1->getX();
		double y1 = p1->getY();
		double x2 = p2->getX();
		double y2 = p2->getY();

		double value = (pow(x - x1, 2) + pow(y - y1, 2)) + (pow(x - x2, 2) + pow(y - y2, 2)) - (pow(x1 - x2, 2) + pow(y1 - y2, 2));

		return value <= tolerance;
	}

	//checks intersection of 2 lines. One line has points (p1, p2) and second line has points (p3, p4)
	shared_ptr<Point> CollisionDetector::lineLineIntersection(shared_ptr<Point> p1, shared_ptr<Point> p2, shared_ptr<Point> p3, shared_ptr<Point> p4)
	{
		double x1 = p1->getX();
		double y1 = p1->getY();
		double x2 = p2->getX();
		double y2 = p2->getY();
		double x3 = p3->getX();
		double y3 = p3->getY();
		double x4 = p4->getX();
		double y4 = p4->getY();

		double px = ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4)) /
			((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));
		double py = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4)) /
			((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));

		return make_shared<Point>(px, py);
	}

	pair<shared_ptr<Uav>, shared_ptr<Uav>> CollisionDetector::getIntersectingUavs(shared_ptr<StateInterface> start, shared_ptr<StateInterface> end)
	{
		for (auto uav1 : start->getUavs())
		{
			for (auto uav2 : start->getUavs())
			{
				if (*uav1.get() != *uav2.get() && areLineSegmentsIntersecting(
					start->getUav(uav1)->getPointParticle()->getLocation(),
					end->getUav(uav1)->getPointParticle()->getLocation(),
					start->getUav(uav2)->getPointParticle()->getLocation(),
					end->getUav(uav2)->getPointParticle()->getLocation()))
				{
					return make_pair(uav1, uav2);
				}
			}
		}
		throw "uavs are not intersecting";
	}

	bool CollisionDetector::checkRelativeLocalization(shared_ptr<StateInterface> node)	//todo: zjistit, zda funguje správnì, pokud je nastaven 1 soused a vypnut swarm splitting
	{
		int number_of_uavs = node->getUavs().size();
		double relative_distance_min = configuration->getRelativeDistanceMin();
		double relative_distance_max = configuration->getRelativeDistanceMax();
		bool check_fov = configuration->getCheckFov();
		// Neighbor must be in certain angle on / off
		double localization_angle = configuration->getLocalizationAngle();
		int required_neighbors = configuration->getRequiredNeighbors();
		bool allow_swarm_splitting = configuration->getAllowSwarmSplitting();

		// Initialize default values
		vector<int> neighbors = vector<int>(number_of_uavs);
		fill(neighbors.begin(), neighbors.end(), 0);	//inicializace

														// Single UAV needs no localization
		if (number_of_uavs == 1)
		{
			return true;
		}

		// Check minimal distance between UAVs
		for (size_t i = 0; i < number_of_uavs - 1; i++)	//todo: zkontrolovat indexy, zda správnì sedí a neutíkají o 1
		{
			for (size_t j = i + 1; j < number_of_uavs; j++)
			{
				auto uavI = node->getUavs()[i]->getPointParticle()->getLocation();
				auto uavJ = node->getUavs()[j]->getPointParticle()->getLocation();

				if (uavI->getDistance(uavJ) <= relative_distance_min)
				{
					return false;
				}
			}
		}

		// Check maximal distance between UAVs
		for (size_t i = 0; i < number_of_uavs - 1; i++)	//todo: zkontrolovat indexy, zda správnì sedí a neutíkají o 1
		{
			for (size_t j = i + 1; j < number_of_uavs; j++)
			{
				auto uavI = node->getUavs()[i]->getPointParticle()->getLocation();
				double uavIphi = node->getUavs()[i]->getPointParticle()->getRotation()->getZ();
				auto uavJ = node->getUavs()[j]->getPointParticle()->getLocation();
				double uavJphi = node->getUavs()[i]->getPointParticle()->getRotation()->getZ();

				if (uavI->getDistance(uavJ) < relative_distance_max && (!check_fov || fabs(uavIphi - uavJphi) < localization_angle / 2))	//fabs je abs pro float
				{
					neighbors[i]++;
					neighbors[j]++;
				}
			}
		}

		bool allUavsHaveNeighbors = false;
		bool oneOrMoreNeighbors = false;
		for (auto neighbor : neighbors)
		{
			allUavsHaveNeighbors = allUavsHaveNeighbors || neighbor >= required_neighbors;
			oneOrMoreNeighbors = oneOrMoreNeighbors || neighbor >= 1;
		}

		// Check whether each UAV has required number of neighbors
		// Pair formations
		if (allow_swarm_splitting)
		{
			return allUavsHaveNeighbors;
		}
		else
			// Whole swarm
		{
			int twoOrMoreNeighbors = 0;
			if (oneOrMoreNeighbors)
			{
				//				char buffer[1024];
				//				sprintf(buffer, "Neighbors: %d %d %d %d \n", neighbors[0], neighbors[1], neighbors[2], neighbors[3]);
				//				logger->logText(buffer);
				for (auto neighbor : neighbors)
				{
					neighbor > 1 ? twoOrMoreNeighbors++ : NULL;
				}
				return twoOrMoreNeighbors >= number_of_uavs - 2;
			}

		}
		return false;
	}

	bool CollisionDetector::insideWorldBounds(shared_ptr<Point> point, int worldWidth, int worldHeight)
	{
		bool inBounds = false;

		if (point->getX() < worldWidth && point->getX() > 0 && point->getY() < worldHeight && point->getY() > 0)
		{
			inBounds = true;
		}
		return inBounds;
	}

	bool CollisionDetector::insideWorldBounds(vector<shared_ptr<Uav>> points, int worldWidth, int worldHeight)
	{
		bool inBounds = true;
		for (auto point : points)
		{
			inBounds = inBounds && insideWorldBounds(point->getPointParticle()->getLocation(), worldWidth, worldHeight);
		}
		return inBounds;
	}

	bool CollisionDetector::isStateValid(shared_ptr<StateInterface> oldState, shared_ptr<StateInterface> newState, shared_ptr<Map> map)
	{
		bool debug = configuration->getDebug();
		bool isStateValid = true;
		if (!checkRelativeLocalization(newState))
		{
			isStateValid = false;
			if (debug)
			{
				logger->logText("node does not comply relative localization");
			}
		}
		if (areTrajectoriesIntersecting(oldState, newState))
		{
			isStateValid = false;
			if (debug)
			{
				logger->logText("node has intersecting trajectories");
			}
		}
		if (!insideWorldBounds(newState->getUavs(), configuration->getWorldWidth(), configuration->getWorldHeight()))
		{
			isStateValid = false;
			if (debug)
			{
				logger->logText("node is outside of world bounds");
			}
		}
		if (!check_obstacle_vcollide_single(newState, map))
		{
			isStateValid = false;
			if (debug)
			{
				logger->logText("node has obstacles in vcollide");
			}
		}
		if (!checkObstaclesInTrajectories(oldState, newState, map))
		{
			isStateValid = false;
			if (debug)
			{
				logger->logText("node has obstacles in trajectories");
			}
		}
		return isStateValid;
	}
	//returns true, when no collisions are detected, returns false, when there are any collisions
	bool CollisionDetector::check_obstacle_vcollide_single(shared_ptr<StateInterface> new_node, shared_ptr<Map> map)
	{
		double uav_size = configuration->getUavSize();
		for (auto uav : new_node->getUavs())
		{
			double x = uav->getPointParticle()->getLocation()->getX();
			double y = uav->getPointParticle()->getLocation()->getY();
			double x1 = x - uav_size / 2;
			double y1 = y - uav_size / 2;
			double z1 = 1;
			double x2 = x + uav_size / 2;
			double y2 = y - uav_size / 2;
			double z2 = 1;
			double x3 = x;
			double y3 = y + uav_size / 2;
			double z3 = 1;

			Triangle3D tri_uav = Triangle3D(Point3D(x1, y1, z1), Point3D(x2, y2, z2), Point3D(x3, y3, z3));

			if (collidesWithObstacles(tri_uav, map))
			{
				return false;
			}
		}
		return true;
	}

	bool CollisionDetector::checkObstaclesInTrajectories(shared_ptr<StateInterface> oldState, shared_ptr<StateInterface> newState, shared_ptr<Map> map)
	{
		double uav_size = configuration->getUavSize();
		for (auto uav : oldState->getUavs())
		{
			double x = uav->getPointParticle()->getLocation()->getX();
			double y = uav->getPointParticle()->getLocation()->getY();
			double x1 = x - uav_size / 2;
			double y1 = y - uav_size / 2;
			double z1 = 1;
			double x2 = x + uav_size / 2;
			double y2 = y + uav_size / 2;
			double z2 = 1;
			double x3 = newState->getUav(uav)->getPointParticle()->getLocation()->getX();
			double y3 = newState->getUav(uav)->getPointParticle()->getLocation()->getY();
			double z3 = 1;

			Triangle3D tri_uav = Triangle3D(Point3D(x1, y1, z1), Point3D(x2, y2, z2), Point3D(x3, y3, z3));
			if (collidesWithObstacles(tri_uav, map))
			{
				return false;
			}

		}
		return true;
	}

	bool CollisionDetector::isInitialSwarmStateFeasible(shared_ptr<StateInterface> state)
	{
		bool debug = configuration->getDebug();
		bool isStateValid = true;
		if (!checkRelativeLocalization(state))
		{
			isStateValid = false;
			if (debug)
			{
				logger->logText("initial swarm position does not comply relative localization");
			}
		}
		return isStateValid;
	}

	void CollisionDetector::setLogger(shared_ptr<LoggerInterface> logger)
	{
		this->logger = logger;
	}

	bool CollisionDetector::collidesWithObstacles(Triangle3D triangle, shared_ptr<Map> map)
	{

		double zero_trans[] = { 0,0,0, 1,0,0, 0,1,0, 0,0,1 };

		for (auto obs : map->getObstacles())
		{
			Point3D p1 = Point3D(obs->rectangle->getX(), obs->rectangle->getY(), 1);
			Point3D p2 = Point3D(obs->rectangle->getX() + obs->rectangle->getWidth(), obs->rectangle->getY(), 1);
			Point3D p3 = Point3D(obs->rectangle->getX() + obs->rectangle->getWidth(), obs->rectangle->getY() + obs->rectangle->getHeight(), 1);
			Point3D p4 = Point3D(obs->rectangle->getX(), obs->rectangle->getY() + obs->rectangle->getHeight(), 1);

			Triangle3D tri1_obs = Triangle3D(p1, p2, p3);
			Triangle3D tri2_obs = Triangle3D(p1, p4, p3);


			bool col = ColDetect::coldetect(triangle, tri1_obs, zero_trans, zero_trans);
			col = col || ColDetect::coldetect(triangle, tri2_obs, zero_trans, zero_trans);
			if (col)
			{
				return true;
			}
		}

		return false;
	}
}