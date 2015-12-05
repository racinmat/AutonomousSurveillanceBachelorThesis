#ifndef _QUADROTOR_UTILS_H_
#define _QUADROTOR_UTILS_H_

#include <cmath>
#include <Eigen/Core>

#include "settings.h"

using namespace imr::settings;

namespace imr {
    namespace constants {
        const double eps = 1e-6;
        const double  pi = 3.14159265358979323846;
        const double   g = 9.81;
    }
    
    namespace defects{
		enum FollowerDefect {FolDefNone, FolDefStatic, FolDefTimedStop, FolDefTimedFault};
		// FolDefNone       = normal function
		// FolDefStatic     = the follower stays on the initial postion
		// FolDefTimedStop  = the follower stops after time t
		// FolDefTimedFault = the follower keeps by the last control input after time t
    }

    namespace robot_types{
        enum Type {UGV,UAV};
    }

    namespace map_initial_control_type{
        enum Initial_type { Line, Manual, RRT, RRT_star};
    }

    namespace utils {
        typedef Eigen::Vector3d Vector;
        typedef Eigen::Vector4d Point;


        #define sqr(x) (double)((x)*(x))
        #define dist(a,b) sqrt(sqr(a(0)-b(0))+sqr(a(1)-b(1))+sqr(a(2)-b(2)))
        #define dist_sqr(a,b) (sqr(a(0)-b(0))+sqr(a(1)-b(1))+sqr(a(2)-b(2)))
        #define dist_sqr2D(a,b) (sqr(a(0)-b(0))+sqr(a(1)-b(1)))
		#define dist2D(a,b) sqrt(sqr(a(0)-b(0))+sqr(a(1)-b(1)))

        #define point2vector(p,v) v(0)=p(0);v(1)=p(1);v(2)=p(2)
        #define vector2point(v,p,t) p(0)=v(0);p(1)=v(1);p(2)=v(2);p(3)=t

        struct robot_state {
			Vector X;
            double phi;

            robot_state() : X(0,0,0), phi(0) {}
            robot_state(const robot_state & st) : X(st.X), phi(st.phi) {}
            robot_state(const Vector _X, const double _phi) : X(_X), phi(_phi) {}
            robot_state(const double _x, const double _y, const double _z, const double _phi) : X(_x,_y,_z), phi(_phi) {}
		};

        struct robot_control {
            double v;
            double w;
            double c;
            double t;

            robot_control() : v(0), w(0), c(0), t(0) {}
            robot_control(const robot_control & rc) : v(rc.v), w(rc.w), c(rc.c), t(rc.t) {}
            robot_control(const double _v, const double _w, const double _c, const double _t) : v(_v), w(_w), c(_c), t(_t) {}
            robot_control(const double _v, const double _w, const double _c) : v(_v), w(_w), c(_c), t(TIME_EXECUTE) {}
		};

        struct robot_desc{
            robot_state state;
            robot_control control;

            robot_desc() : state(), control() {}
            robot_desc(const robot_state & rs, const robot_control & rc) : state(rs), control(rc) {}
        };

        struct robot_limits {
            double vMax;
            double wMax;
            double cMax;

            double vMin;
            double wMin;
            double cMin;

            robot_limits() : vMax(0), wMax(0), cMax(0), vMin(0), wMin(0), cMin(0) {}
            robot_limits(const imr::robot_types::Type &type){
                switch (type) {
                case robot_types::UAV:
                    vMax=UAV_V_MAX; wMax=UAV_W_MAX; cMax=UAV_CURV_MAX;
                    vMin=UAV_V_MIN; wMin=UAV_W_MIN; cMin=UAV_CURV_MIN;
                    break;
                case robot_types::UGV:
                    vMax=UGV_V_MAX; cMax=UGV_CURV_MAX; wMax=0;
                    vMin=UGV_V_MIN; cMin=UGV_CURV_MIN; wMin=0;
                    break;
                }
            }
        };
		
		struct formation_spec {
			double p;
			double q;
            double h;

            formation_spec() : p(0.0), q(0.0), h(0.0) {}
            formation_spec(const formation_spec & fs) : p(fs.p), q(fs.q), h(fs.h) {}
            formation_spec(const double _p, const double _q, const double _h) : p(_p), q(_q), h(_h) {}
		};

    }
}
#endif
