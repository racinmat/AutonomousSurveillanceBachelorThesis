#ifndef __TYPES_H__
#define __TYPES_H__

#include <vector>
#include "utils.h"

typedef imr::utils::Point Point;
typedef imr::utils::Vector Vector;
typedef imr::utils::formation_spec FSpec;
typedef imr::utils::robot_state State;
typedef imr::utils::robot_limits Limits;
typedef imr::utils::robot_control Control;
typedef imr::utils::robot_desc Desc;

// Used in Obstacle - holding obstacle nodes
typedef std::vector<Point> Points;
typedef std::vector<Vector> Vectors;

// For purposes of formation driving
typedef std::vector<Desc> Trajectory;

// Logging of the simulation
typedef std::vector<State> States;
typedef std::vector<Control> Controls;

// For purposes of simulation of planner
typedef std::vector<Controls> Planner_controls;
#endif
