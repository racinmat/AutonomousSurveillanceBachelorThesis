#include "CollisionDetector.h"
#include "Configuration.h"
#include "VCollide/Triangle3D.h"
#include "VCollide/ColDetect.h"
#include <boost/numeric/ublas/matrix_proxy.hpp>

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
		for (auto uav1 : start->getBaseUavs())
		{
			for (auto uav2 : start->getBaseUavs())
			{
				if (*uav1.get() != *uav2.get() && areLineSegmentsIntersecting(
					start->getBaseUav(uav1)->getPointParticle()->getLocation(),
					end->getBaseUav(uav1)->getPointParticle()->getLocation(),
					start->getBaseUav(uav2)->getPointParticle()->getLocation(),
					end->getBaseUav(uav2)->getPointParticle()->getLocation()))
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

	bool CollisionDetector::areLineSegmentsIntersecting(shared_ptr<UavInterface> uav1start, shared_ptr<UavInterface> uav1end, shared_ptr<UavInterface> uav2start, shared_ptr<UavInterface> uav2end)
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

	pair<shared_ptr<UavInterface>, shared_ptr<UavInterface>> CollisionDetector::getIntersectingUavs(shared_ptr<StateInterface> start, shared_ptr<StateInterface> end)
	{
		for (auto uav1 : start->getBaseUavs())
		{
			for (auto uav2 : start->getBaseUavs())
			{
				if (*uav1.get() != *uav2.get() && areLineSegmentsIntersecting(
					start->getBaseUav(uav1)->getPointParticle()->getLocation(),
					end->getBaseUav(uav1)->getPointParticle()->getLocation(),
					start->getBaseUav(uav2)->getPointParticle()->getLocation(),
					end->getBaseUav(uav2)->getPointParticle()->getLocation()))
				{
					return make_pair(uav1, uav2);
				}
			}
		}
		throw "uavs are not intersecting";
	}

	bool CollisionDetector::checkRelativeLocalization(shared_ptr<StateInterface> node)	//todo: zjistit, zda funguje správnì, pokud je nastaven 1 soused a vypnut swarm splitting
	{
		int number_of_uavs = node->getBaseUavs().size();
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
				auto uavI = node->getBaseUavs()[i]->getPointParticle()->getLocation();
				auto uavJ = node->getBaseUavs()[j]->getPointParticle()->getLocation();

				if (uavI->getDistance(uavJ) <= relative_distance_min)
				{
					return false;
				}
			}
		}

		auto uavAdjacencyMatrix = ublas::matrix<int>(number_of_uavs, number_of_uavs, 0);

		// Check maximal distance between UAVs
		for (size_t i = 0; i < number_of_uavs - 1; i++)	//todo: zkontrolovat indexy, zda správnì sedí a neutíkají o 1
		{
			for (size_t j = i + 1; j < number_of_uavs; j++)
			{
				auto uavI = node->getBaseUavs()[i]->getPointParticle()->getLocation();
				double uavIphi = node->getBaseUavs()[i]->getPointParticle()->getRotation()->getZ();
				auto uavJ = node->getBaseUavs()[j]->getPointParticle()->getLocation();
				double uavJphi = node->getBaseUavs()[j]->getPointParticle()->getRotation()->getZ();

				if (uavI->getDistance(uavJ) < relative_distance_max && (!check_fov || fabs(uavIphi - uavJphi) < localization_angle / 2))	//fabs je abs pro float
				{
					neighbors[i]++;
					neighbors[j]++;
					uavAdjacencyMatrix(i, j) = 1;
					uavAdjacencyMatrix(j, i) = 1;
				}
			}
		}

		//TODO: zkontrolovat, zda jsou UAV celistvým øetìzem, pomocí DFS nebo BFS prohledat celý graf (graf sestavím z adjacency matrix tak, že 1 je tam, kde jsou UAV ve správné vzdálenosti a jinde je 0)

		bool allUavsHaveNeighbors = true;		//is true when each uav has required amount of required neighbours or more
		for (auto neighbor : neighbors)
		{
			allUavsHaveNeighbors = allUavsHaveNeighbors && neighbor >= required_neighbors;
		}

		// Check whether each UAV has required number of neighbors
		// Pair formations
		if (allow_swarm_splitting)
		{
			return allUavsHaveNeighbors;
		}
		else
		{
			// Whole swarm
			return isGraphConnected(uavAdjacencyMatrix) && allUavsHaveNeighbors;
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

	bool CollisionDetector::insideWorldBounds(vector<shared_ptr<UavInterface>> points, int worldWidth, int worldHeight)
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
		if (!insideWorldBounds(newState->getBaseUavs(), configuration->getWorldWidth(), configuration->getWorldHeight()))
		{
			isStateValid = false;
			if (debug)
			{
				logger->logText("node is outside of world bounds");
			}
		}
		//when using obstacles amplifying and colliding the whole trajectory with width same as UAV width, new state does not have collisions, when whole trajectory to new state does not have collisions
//		if (!checkCollisionsInNewState(newState, map))
//		{
//			isStateValid = false;
//			if (debug)
//			{
//				logger->logText("node has obstacles in vcollide");
//			}
//		}
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
	bool CollisionDetector::checkCollisionsInNewState(shared_ptr<StateInterface> new_node, shared_ptr<Map> map)
	{
		//TODO: zamyslet se, jestli neslouèit tuto metodu a checkObstaclesInTrajectories do jedné, protože øešení kolizí trvá neskuteènì dlouho
		double uav_size = configuration->getUavSize();
		for (auto uav : new_node->getBaseUavs())
		{
			auto uavLocation = uav->getPointParticle()->getLocation();
			auto rotation = uav->getPointParticle()->getRotation()->getZ();

			double x = uavLocation->getX();//coordinates of UAV when UAV has rotation = 0
			double y = uavLocation->getY();//UAV is approximated as square
			double coef = sqrt(2) * (uav_size / 2);	//distance of corner from middle is sqrt(2) * size of uav/2


			Rectangle2D uavRectangle(x - uav_size / 2, y - uav_size / 2, uav_size, uav_size);
			uavRectangle.rotate(rotation);
			if (collidesWithObstacles(uavRectangle, map))
			{
				return false;
			}
		}
		return true;
	}

	bool CollisionDetector::checkObstaclesInTrajectories(shared_ptr<StateInterface> oldState, shared_ptr<StateInterface> newState, shared_ptr<Map> map)
	{
		double uav_size = configuration->getUavSize();
		for (auto uav : oldState->getBaseUavs())
		{
			//trajectory of UAV between 2 states is approximed by line. When approximating UAV as square, we get rectangle, where can not be collisions
			//by knowing these 2 states and size of UAV square, I count rectangle coords and rotate whole rectangle

			/*

			                                                                           `..
			                                                                        `````` rotated rectangle
			                                                                     `````````
			                                                                  .```````````.
			                                                               .```````````````
			                                                            .``````````````````
			                                                         .`````````````````````.
			                                                      .`````````````````````````
			                                                   .````````````````````````````
			                                                `.````````````````````````````````
			                                             `.```````````````````````````````````
			not rotated rectangle                     `.``````````````````````````````````````
			,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
			``````````````````````````````````` ..````````````````````````````````````````````.`
			```````````````````````````````` ..```````````````````````````````newLocation``````
			``````````````````````````````..`````````````````````````````````````,'`````````````
			```````````````````````````..`````````````````````````````````````,,```````````````.
			````````````````````````..`````````````````````````````````````,.```````````````````
			`````````````````````..`````````````````````````````````````,.```````````````````````
			``````````````````.``````````````````````````````````````,.`````````````````````````.
			```````````````.``````````````````````````````````````,.`````````````````````````````
			````````````.``````````````````````````````````````,.`````````````````````````````````
			`````````.``````````````````````````````````````,.```````````````````````````````````.
			``````.``````````````````````````````````````,````````````````````````````````````.`
			```.``````````````````````````````````middle```````````````````````````````````.````
			`,`````````````````````````````````````.,````````````````````````````````````.```````
			.`.``````````````````````````````````.,````````````````````````````````````.``````````
			`.```````````````````````````````.,````````````````````````````````````.`````````````
			`.````````````````````````````.,````````````````````````````````````.````````````````
			..`````````````````````````.,````````````````````````````````````.```````````````````
			.``````````````````````.,````````````````````````````````````.``````````````````````
			.```````````````````,,````````````````````````````````````.`````````````````````````
			,````````````````,.````````````````````````````````````.````````````````````````````
			`````````````',.````````````````````````````````````.```````````````````````````````
			`````````````,```````````````````````````````````.``````````````````````````````````
			`.````````oldLocation`````````````````````````.` ```````````````````````````````````
			```````````````````````````````````````````..```````````````````````````````````````
			  ````````````````````````````````````` .`
			  .``````````````````````````````````.`
			    ```````````````````````````````.`
			    ````````````````````````````.`
			    .````````````````````````.`
			      ```````````````````````
			      ``````````````````.`
			      .``````````````.`
			       ```````````.`
			       ``````````
			       .``````
			       ```

			*/	

			auto oldUavLocation = uav->getPointParticle()->getLocation();
			auto newUavLocation = newState->getBaseUav(uav)->getPointParticle()->getLocation();
			auto distance = oldUavLocation->getDistance(newUavLocation);

			double xOld = oldUavLocation->getX();
			double yOld = oldUavLocation->getY();
			double xNew = newUavLocation->getX();
			double yNew = newUavLocation->getY();

			double middleX = (xOld + xNew) / 2;
			double middleY = (yOld + yNew) / 2;

			Rectangle2D uavRectangle(middleX - distance / 2 - uav_size / 2, middleY - uav_size / 2, distance + uav_size, uav_size);
			uavRectangle.rotate(atan2(yNew - yOld, xNew - xOld));
			if (collidesWithObstacles(uavRectangle, map))
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

	bool CollisionDetector::collidesWithObstacles(Rectangle2D rectangle, shared_ptr<Map> map)
	{

		for (auto obs : map->getObstacles())
		{
			auto obstacleRectangle = obs->rectangle->toColDetectRectandle();
			auto collision = ColDetect::coldetectWithoutTransformation(obstacleRectangle, rectangle);
			if (collision)
			{
				return true;
			}
		}

		return false;
	}

	// if graph is connected, all uavs are in one swarm.
	// adjacency matrix is built from feasible UAV distances. If UAVS have distance between min and max, edge is between them, otherwise, edge is not between them.
	// Thus graph is created, represented by adjacency matrix
	// DFS determines whether graph is connected or not. If all nodes are discovered, graph is connected
	bool CollisionDetector::isGraphConnected(ublas::matrix<int> adjacencyMatrix)
	{
		vector<int> visited = vector<int>();	//array of indexes

		vector<int> opened = vector<int>();		//array of indexes
		vector<bool> closed = vector<bool>(adjacencyMatrix.size1());		//index is node, bool is closed
		fill(closed.begin(), closed.end(), false);		// Mark all the vertices as not closed



		int currentNode;	//I start searching from first UAV, node is represented by index
		opened.push_back(0);
		visited.push_back(0);

		bool noneOpened = false;
		while (!noneOpened)
		{
			currentNode = *opened.begin();	//pick last element
			opened.erase(opened.begin());	//remove last element

			closed[currentNode] = true;
			auto currentNodeRow = row(adjacencyMatrix, currentNode);
			for (size_t i = 0; i < currentNodeRow.size(); i++)
			{
				bool isNodeOpened = find(opened.begin(), opened.end(), i) != opened.end();
				if (currentNodeRow[i] == 1 && !closed[i] && !isNodeOpened)
				{
					opened.push_back(i);
					visited.push_back(i);
				}
			}

			noneOpened = opened.empty();
		}

		return visited.size() == adjacencyMatrix.size1();	//if count of visited elements id equal to size of matrix, all elements were visited and graph is connected
	}

}