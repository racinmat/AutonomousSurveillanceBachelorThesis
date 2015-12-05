#include "PathOptimizer.h"

namespace App
{


	PathOptimizer::PathOptimizer()
	{
	}


	PathOptimizer::~PathOptimizer()
	{
	}

	//optimalizuje cestu pomocí Dubinsových manévrù
	vector<shared_ptr<State>> PathOptimizer::optimizePath(vector<shared_ptr<State>> path)
	{
	}


	/**

	* @brief Calculate state of robot from position "pos" and action described by control "cnt".

	* @param pos       - Initial State of the robot

	* @param cnt       - Controls of action

	* @return new State of the robot

	*/

//	State Robot::calculate_state(const State & pos, const Control & cnt) {
//
//		State ret;
//
//		if (abs(cnt.c)<imr::constants::eps) {
//
//			ret.X(0) = pos.X(0) + cnt.v*cos(pos.phi)*cnt.t;
//
//			ret.X(1) = pos.X(1) + cnt.v*sin(pos.phi)*cnt.t;
//
//		}
//		else {
//
//			ret.X(0) = pos.X(0) + (sin(pos.phi + cnt.c*cnt.v*cnt.t) - sin(pos.phi)) / cnt.c;
//
//			ret.X(1) = pos.X(1) - (cos(pos.phi + cnt.c*cnt.v*cnt.t) - cos(pos.phi)) / cnt.c;
//
//		}
//
//		ret.X(2) = pos.X(2) + cnt.w*cnt.t;
//
//		ret.phi = pos.phi + cnt.c*cnt.v*cnt.t;
//
//		return ret;
//
//	}

}
