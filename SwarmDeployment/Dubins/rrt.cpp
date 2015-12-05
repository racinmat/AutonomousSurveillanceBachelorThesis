#include "rrt.h"

#include "../settings.h"

#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include "../measurement/time_measurement.h"
#include "types.h"


typedef imr::time::TimeMeasurement TimeMeasurement;


using namespace imr::rrt_algorithm::rrt;
using namespace imr::settings;
using namespace imr::constants;


RRT::RRT(const State &_state, const double _time): time(_time)
{
    /// Tree initialization
    nodes.reserve(RRT_MAX_ITER);
    states.reserve(RRT_MAX_ITER);
    states.push_back(_state);

    const int dimension = 2;
    int *topology = new int[dimension];
    MPNN::ANNcoord *scale = new MPNN::ANNcoord[dimension];
    for(int i=0;i<dimension;i++) {
        scale[i] = 1.0;
        topology[i] = 1;
    }

    kdTree = new MPNN::MultiANN<int>(dimension,1,topology,(MPNN::ANNpoint)scale);
    generate_input_seed();
    TimeMeasurement tm;
    tm.start();
    construct_tree();
    find_trajectory();
    tm.end();
}

RRT::~RRT()
{}

Controls RRT::get_new_trajectory()
{
    construct_tree();
    construct_trajectory();
    return get_trajectory();
}

void RRT::generate_input_seed(){
    Limits limits = world.leader->get_limits();
    /// Forward-Left
    cnt_N.push_back(Control(0.8*limits.vMax,0,limits.cMax,TIME_EXECUTE));
    //cnt_N.push_back(Control(0.8*limits.vMax,0,limits.cMax/2,TIME_EXECUTE));

    /// Forward-Straight
    cnt_N.push_back(Control(0.8*limits.vMax,0,0,TIME_EXECUTE));

    /// Forward-Right
    cnt_N.push_back(Control(0.8*limits.vMax,0,limits.cMin,TIME_EXECUTE));
    //cnt_N.push_back(Control(0.8*limits.vMax,0,limits.cMin/2,TIME_EXECUTE));


#ifdef ENABLE_NEGATIVE_VELOCITY
    /// Rearward-Left
    cnt_N.push_back(Control(0.6*limits.vMin,0,limits.cMax,TIME_EXECUTE));
    //cnt_N.push_back(Control(0.6*limits.vMin,0,limits.cMax/2,TIME_EXECUTE));

    /// Rearward-Straight
    cnt_N.push_back(Control(0.6*limits.vMin,0,0,TIME_EXECUTE));

    /// Rearward-Right
    cnt_N.push_back(Control(0.6*limits.vMin,0,limits.cMin,TIME_EXECUTE));
    //cnt_N.push_back(Control(0.6*limits.vMin,0,limits.cMin/2,TIME_EXECUTE));

#endif

    /// Forward-Left
    cnt_M.push_back(Control(0.8*limits.vMax,0,limits.cMax,RRT_TIME_STEP));
    //cnt_M.push_back(Control(0.8*limits.vMax,0,limits.cMax/2,RRT_TIME_STEP));

    /// Forward-Straight
    cnt_M.push_back(Control(0.8*limits.vMax,0,0,RRT_TIME_STEP));

    /// Forward-Right
    cnt_M.push_back(Control(0.8*limits.vMax,0,limits.cMin,RRT_TIME_STEP));
    //cnt_M.push_back(Control(0.8*limits.vMax,0,limits.cMin/2,RRT_TIME_STEP));


#ifdef ENABLE_NEGATIVE_VELOCITY
    /// Rearward-Left
    cnt_M.push_back(Control(0.6*limits.vMin,0,limits.cMax,RRT_TIME_STEP));
    //cnt_M.push_back(Control(0.6*limits.vMin,0,limits.cMax/2,RRT_TIME_STEP));

    /// Rearward-Straight
    cnt_M.push_back(Control(0.6*limits.vMin,0,0,RRT_TIME_STEP));

    /// Rearward-Right
    cnt_M.push_back(Control(0.6*limits.vMin,0,limits.cMin,RRT_TIME_STEP));
    //cnt_M.push_back(Control(0.6*limits.vMin,0,limits.cMin/2,RRT_TIME_STEP));

#endif
}

Vector RRT::sample_target()
{   const double rnd = double(rand()) / double(RAND_MAX);
    if (rnd <RRT_GOAL_PROBABILITY){
#ifdef DEBUG_RRT_RND
        std::cout << "RRTDEBUG: Rand configuration - GOAL" << std::endl;
#endif
        return Vector(imr::settings::goal[0],imr::settings::goal[1],imr::settings::goal[2]);
    }
    Vector rand_config = world.map->get_random_position_in_map();
#ifdef DEBUG_RRT_RND
    std::cout << "RRTDEBUG: Rand configuration [" << rand_config.transpose() << "]" << std::endl;
#endif
    return rand_config;
}

RRT::node* RRT::extend(const size_t nearest, const Vector &target, bool *status)
{
    State s_new;
    State candidate;
    double min_dist = std::numeric_limits<double>::max();
    double dist;
    bool valid;
    int input;
    double wall_dist, obst_dist = std::numeric_limits<double>::max();
    
    for (int i=0; i < cnt_M.size(); i++) {
        /// Calculate candidate position
        if (nodes[nearest]->t<LEADER_N_SAMPLES*TIME_EXECUTE)
            candidate = world.leader->calculate_state(states[nearest], cnt_N[i]);
        else
            candidate = world.leader->calculate_state(states[nearest], cnt_M[i]);
        /// Distance to random node -> the closest node should be picked
        dist = dist_sqr2D(candidate.X, target);
        
        if (min_dist > dist) {
            /// Check if it is not in the succesor set of s_near, allready
            valid = true;
            for (int j=0; j<nodes[nearest]->children.size(); j++) {
                valid &= dist_sqr2D(candidate.X, states[ nodes[nearest]->children[j] ].X) > eps;
            }
            
            if (valid) {
                /// Check for collisions
                wall_dist = world.map->wall_distance(Point(candidate.X(0),candidate.X(1),candidate.X(2),0));

                if (nodes[nearest]->t<LEADER_N_SAMPLES*TIME_EXECUTE)
                    obst_dist = world.map->obstacle_distance(Point(candidate.X(0),candidate.X(1),candidate.X(2),nodes[nearest]->t+TIME_EXECUTE),time);
                else
                    obst_dist = world.map->obstacle_distance(Point(candidate.X(0),candidate.X(1),candidate.X(2),nodes[nearest]->t+RRT_TIME_STEP),time);

                if (obst_dist > world.leader->get_ra() && wall_dist > world.leader->get_ra()) {
                    min_dist = dist;
                    s_new = candidate;
                    input = i;
                }
            }
        }
    }

    node *rv;
    if (min_dist < std::numeric_limits<double>::max()) {
        *status = true;
        if (nodes[nearest]->t<LEADER_N_SAMPLES*TIME_EXECUTE)
            rv = new node(nearest, input,nodes[nearest]->t+TIME_EXECUTE);
        else
            rv = new node(nearest, input,nodes[nearest]->t+RRT_TIME_STEP);
    } else {
        *status = false;
    }

    return rv;
}

void RRT::create_KDTree(){
    for (int i = 0; i < states.size(); ++i) {
        add_point_to_KDTree(states[i],i);
    }
}

void RRT::add_point_to_KDTree(const State &_state, const int index){
    MPNN::ANNpoint annpt = MPNN::annAllocPt(2);
    annpt[0] = _state.X(0);
    annpt[1] = _state.X(1);
    kdTree->AddPoint(annpt,index);
}

int RRT::find_NN_in_KDTree(const Vector _point){
    int idx;
    double dann = INFINITY;
    MPNN::ANNpoint query = MPNN::annAllocPt(2);
    query[0] = _point(0);
    query[1] = _point(1);
    int nearest = (int)kdTree->NearestNeighbor(query,idx,dann);
    MPNN::annDeallocPt(query);
    return nearest;
}

void RRT::construct_tree()
{
    bool run = true;
    int lcnt = 0;
    bool new_config_status = true;
    double d2g;
    State st;
    clear_tree();
    create_KDTree();

    //std::srand(std::time(0));
    /// Tree construction
#ifdef DEBUG_RRT
    std::cout << "RRT: running tree construction ..." << std::endl;
#endif
    while ( run ) {
        const Vector rand = sample_target();
        const size_t near = find_NN_in_KDTree(rand);
        node* n_new = extend(near, rand, &new_config_status);
        if (new_config_status == true) {
            if (n_new->t<=LEADER_N_SAMPLES*TIME_EXECUTE)
                st = world.leader->calculate_state(states[near], cnt_N[n_new->control]);
            else
                st = world.leader->calculate_state(states[near], cnt_M[n_new->control]);
            nodes[near]->children.push_back(nodes.size());
            nodes.push_back(n_new);
            states.push_back(st);
            add_point_to_KDTree(st,states.size()-1);
            d2g = world.map->goal_distance(st.X);
            if (d2g < CONSTR_DISTANCE_TO_GOAL) run = false;
        }
        if (++lcnt == RRT_MAX_ITER) run = false;
    }
#ifdef DEBUG_RRT
    std::cout << "RRT: tree constructed: " << endl;
    cout << "   D2G:   " << d2g << endl;
    cout << "   Size:  " << nodes.size()-1 << endl;
    cout << "   Iters: " << lcnt   << std::endl;
#endif
}

void RRT::find_trajectory()
{
#ifdef DEBUG_RRT
    std::cout << "RRT: costructing trajectory ..." << std::endl;
#endif
    size_t q_last = find_NN_in_KDTree(world.map->get_goal());
    trajectory_quality = world.map->goal_distance(states[q_last].X);
    trajectory.clear();
    trajectory.push_back(q_last);
    while (nodes[q_last]->state_id != -1) {
        trajectory.insert(trajectory.begin(), nodes[q_last]->state_id);
        q_last = nodes[q_last]->state_id;
    }
    trajectory.erase(trajectory.begin(),trajectory.begin()+1);
    out_trajectory = construct_trajectory();

#ifdef DEBUG_RRT
    std::cout << "RRT: trajectory constructed ... Size: " << out_trajectory.size() <<  std::endl;
#endif
}

Controls RRT::construct_trajectory()
{
    Controls new_trajectory;
    const int n = min(LEADER_N_SAMPLES,int(trajectory.size()));
    for (int i = 0; i < n; ++i) {
        new_trajectory.push_back(cnt_N[nodes[trajectory[i]]->control]);
    }

    for (int i = new_trajectory.size(); i < LEADER_N_SAMPLES; ++i) {
        new_trajectory.push_back(Control(0,0,0,TIME_EXECUTE));
    }

    if (trajectory.size()<=LEADER_N_SAMPLES) return new_trajectory;

    for(int i = LEADER_N_SAMPLES; i<trajectory.size(); ++i) {
        new_trajectory.push_back(cnt_M[nodes[trajectory[i]]->control]);
    }
    return new_trajectory;
}

void RRT::simplify_trajectory()
{
    const double v_tol = 0.01;
    const double w_tol = 0.01;
    const double c_tol = 0.01;
    Controls new_trajectory;

    // colecting together similar parts of a trajectory
    new_trajectory.push_back(out_trajectory[0]);
    int curr = 0;
    double t;
    for(int i = 1; i < out_trajectory.size(); ++i) {
        if (fabs(out_trajectory[i].v-new_trajectory[curr].v) < v_tol &&
                fabs(out_trajectory[i].w-new_trajectory[curr].w) < w_tol &&
                fabs(out_trajectory[i].c-new_trajectory[curr].c) < c_tol)
        {
            if (new_trajectory[curr].t + out_trajectory[i].t <= LEADER_TIME_STEP_MAX){
                new_trajectory[curr].t += out_trajectory[i].t;
            }else{
                t = new_trajectory[curr].t + out_trajectory[i].t-LEADER_TIME_STEP_MAX;
                new_trajectory[curr].t = LEADER_TIME_STEP_MAX;
                new_trajectory.push_back(out_trajectory[i]);
                new_trajectory.back().t = t;
                ++curr;
            }
        } else {
            new_trajectory.push_back(out_trajectory[i]);
            ++curr;
        }
    }

    out_trajectory = new_trajectory;
    new_trajectory.clear();

    // dividing the trajectory in correct meaning for the MPC
    /*
    double total_t = 0;
    for (int i = 0; i < out_trajectory.size(); ++i) {
        total_t += out_trajectory[i].t;
    }
    const int n_execute = int(total_t / TIME_EXECUTE + 0.5);
    const int n = min(n_execute, LEADER_N_SAMPLES);

    for (int i = 0; i < n; ++i) {
        new_trajectory.push_back(cnt_N[nodes[trajectory[i]]->control]);
    }

    for (int i = new_trajectory.size(); i < LEADER_N_SAMPLES; ++i) {
        new_trajectory.push_back(Control(0,0,0,TIME_EXECUTE));
    }

    if (trajectory.size()<=LEADER_N_SAMPLES) return new_trajectory;
    */

#ifdef DEBUG_RRT
    std::cout << "RRT: trajectory simplified ... Size: " << out_trajectory.size() <<  std::endl;
#endif
}

void RRT::optimize_trajectory(){
    Trajectory traj;
    double length, new_length;
    double v_max = world.leader->get_limits().vMax*0.8;
    double c_max = world.leader->get_limits().cMax;
    double c_min = world.leader->get_limits().cMin;
    State st = states[0],
            st2;
    for (int i = 0; i < out_trajectory.size(); ++i) {
        traj.push_back(Desc(st,out_trajectory[i]));
        st2 = world.leader->calculate_state(st, out_trajectory[i]);
        st = st2;
    }
    traj.push_back(Desc(st,Control()));

#ifdef DEBUG_RRT
    length = 0;
    for (int i = 0; i < traj.size(); ++i) {
        length += traj[i].control.t * traj[i].control.v;
    }
    TimeMeasurement tm;
    tm.start();
    std::cout << "RRT: original trajectory length: " << length <<  std::endl;
    /*
    std::cout << std::endl << "LEADER PLANNED POSITIONS:" << std::endl;
    for (int i = 0; i < traj.size(); ++i) {
        std::cout << "	X = [ " << traj[i].state.X(0) << ", " << traj[i].state.X(1) << ", " << traj[i].state.X(2) << " ]" << std::endl;
    }
*/
#endif
    int loop = 0;
    bool isValid;
    bool endPoint;
    while(loop < 100){
        ++loop;
        int n_first = rand() % traj.size();
        int n_second = rand() % traj.size();
        while(n_first == n_second){
            n_second = rand() % traj.size();
        }

        if (n_first > n_second){
            const int p = n_first;
            n_first = n_second;
            n_second = p;
        }

        length = 0;
        for (int i = n_first; i < n_second; ++i) {
            length += traj[i].control.t * fabs(traj[i].control.v);
        }
        endPoint = false;
        if (n_second != traj.size()-1){
            dubins = new geom::Dubins(geom::Position(geom::Point(traj[n_first].state.X(0),traj[n_first].state.X(1)), traj[n_first].state.phi),
                                      geom::Position(geom::Point(traj[n_second].state.X(0),traj[n_second].state.X(1)), traj[n_second].state.phi),
                                      1/world.leader->cMax());
        }else{
            endPoint = true;
            dubins = new geom::Dubins(geom::Position(geom::Point(traj[n_first].state.X(0),traj[n_first].state.X(1)), traj[n_first].state.phi),
                                      geom::Point(goal[0],goal[1]),
                    1/world.leader->cMax());
            length += dist2D(traj.back().state.X,Vector(goal[0],goal[1],0));
        }
        new_length = dubins->getLength();

        if (new_length < length){
#ifdef DEBUG_RRT_DUBINS
            if (endPoint){
            std::cout << "DUBINS: original part length   : " << length <<  std::endl;
            std::cout << "        simplified part length : " << new_length <<  std::endl;
            std::cout << "        type of Dubins maneuver: " << dubins->getTypeOfManeuver() << " [RSR, LSL, LSR, RSL, RLR, LRL, LS, RS]" << std::endl;
            }
#endif

            Trajectory new_traj;
            for (int i = 0; i < n_first; ++i) {
                new_traj.push_back(traj[i]);
            }
            double t1, t2, t3, c1, c2, c3;
            State state_first = traj[n_first].state;
            Control cnt;
            switch (dubins->getTypeOfManeuver()) {
            case geom::RSR:
                t1 = dubins->getRadius()*fabs(dubins->getLen1())/v_max;
                t2 = dubins->getLen2()/v_max;
                t3 = dubins->getRadius()*fabs(dubins->getLen3())/v_max;
                c1 = c_min;
                c2 = 0;
                c3 = c_min;
                break;
            case geom::LSL:
                t1 = dubins->getRadius()*fabs(dubins->getLen1())/v_max;
                t2 = dubins->getLen2()/v_max;
                t3 = dubins->getRadius()*fabs(dubins->getLen3())/v_max;
                c1 = c_max;
                c2 = 0;
                c3 = c_max;
                break;
            case geom::LSR:
                t1 = dubins->getRadius()*fabs(dubins->getLen1())/v_max;
                t2 = dubins->getLen2()/v_max;
                t3 = dubins->getRadius()*fabs(dubins->getLen3())/v_max;
                c1 = c_max;
                c2 = 0;
                c3 = c_min;
                break;
            case geom::RSL:
                t1 = dubins->getRadius()*fabs(dubins->getLen1())/v_max;
                t2 = dubins->getLen2()/v_max;
                t3 = dubins->getRadius()*fabs(dubins->getLen3())/v_max;
                c1 = c_min;
                c2 = 0;
                c3 = c_max;
                break;
            case geom::RLR:
                t1 = dubins->getRadius()*fabs(dubins->getLen1())/v_max;
                t2 = dubins->getRadius()*fabs(dubins->getLen2())/v_max;
                t3 = dubins->getRadius()*fabs(dubins->getLen3())/v_max;
                c1 = c_min;
                c2 = c_max;
                c3 = c_min;
                break;
            case geom::LRL:
                t1 = dubins->getRadius()*fabs(dubins->getLen1())/v_max;
                t2 = dubins->getRadius()*fabs(dubins->getLen2())/v_max;
                t3 = dubins->getRadius()*fabs(dubins->getLen3())/v_max;
                c1 = c_max;
                c2 = c_min;
                c3 = c_max;
                break;
            default:
                break;
            }

            State s_new, s_old = state_first;
            int n_added = 0;
            if (t1 > eps){
                while (t1-RRT_TIME_STEP>eps){
                    cnt = Control(v_max,0,c1,RRT_TIME_STEP);
                    new_traj.push_back(Desc(s_old,cnt));
                    t1 -= RRT_TIME_STEP;

                    s_new = world.leader->calculate_state(s_old, cnt);
                    s_old = s_new;
                    ++n_added;
                }
                cnt = Control(v_max,0,c1,t1);
                new_traj.push_back(Desc(s_old,cnt));
                s_new = world.leader->calculate_state(s_old, cnt);
                s_old = s_new;
                ++n_added;
            }

            if (t2 > eps){
                while (t2-RRT_TIME_STEP>eps){
                    cnt = Control(v_max,0,c2,RRT_TIME_STEP);
                    new_traj.push_back(Desc(s_old,cnt));
                    t2 -= RRT_TIME_STEP;

                    s_new = world.leader->calculate_state(s_old, cnt);
                    s_old = s_new;
                    ++n_added;
                }
                cnt = Control(v_max,0,c2,t2);
                new_traj.push_back(Desc(s_old,cnt));
                s_new = world.leader->calculate_state(s_old, cnt);
                s_old = s_new;
                ++n_added;
            }
            if (t3 > eps){
                while (t3-RRT_TIME_STEP>eps){
                    cnt = Control(v_max,0,c3,RRT_TIME_STEP);
                    new_traj.push_back(Desc(s_old,cnt));
                    t3 -= RRT_TIME_STEP;

                    s_new = world.leader->calculate_state(s_old, cnt);
                    s_old = s_new;
                    ++n_added;
                }
                cnt = Control(v_max,0,c3,t3);
                new_traj.push_back(Desc(s_old,cnt));
                ++n_added;
            }

            isValid = true;
            for (int i = 0; i < n_added; ++i) {
                const int index = new_traj.size()-n_added+i;
                Points pts = world.leader->getSampledTrajectory(new_traj[index].state,&new_traj[index].control,1);
                for (int j = 0; j < pts.size(); ++j) {
                    isValid &= world.map->wall_distance(pts[j]) > world.leader->get_ra();
                    if (!isValid)
                        break;
                }
                if (!isValid)
                    break;
            }
            if (!isValid){
                continue;
            }

            loop = 0;
            if (!endPoint){
                for (int i = n_second; i < traj.size(); ++i) {
                    new_traj.push_back(traj[i]);
                }
            }else{
                s_new = world.leader->calculate_state(new_traj.back().state, new_traj.back().control);
                new_traj.push_back(Desc(State(goal[0],goal[1],0,0),Control()));
            }
            traj = new_traj;

        }
    }
    out_trajectory.clear();
    for (int i = 0; i < traj.size()-1; ++i) {
        out_trajectory.push_back(traj[i].control);
    }

#ifdef DEBUG_RRT
    length = 0;
    for (int i = 0; i < traj.size(); ++i) {
        length += traj[i].control.t * traj[i].control.v;
    }
    tm.end();
    std::cout << "RRT: optimized trajectory length: " << length <<  std::endl;
    std::cout << "                            size: " << traj.size()-1 <<  std::endl;

    /*
    std::cout << std::endl;
    for (int i = 0; i < traj.size(); ++i) {
        std::cout << "   v = " << traj[i].control.v << ", w = " << traj[i].control.w << ", c = " << traj[i].control.c << ", t = " << traj[i].control.t << std::endl;
    }
    */
#endif

}



void RRT::clear_tree() {
    //kdTree->ResetMultiANN();
    nodes.clear();
    nodes.push_back(new node());
    State st = states[0];
    states.clear();
    states.push_back(st);
    trajectory.clear();
}

void RRT::save_tree(const char *filename)
{
    std::ofstream ofs(filename);
    ofs << "function [tree]= tree()" << endl;
    ofs << "% Positions of parent : X = (x, y, z, phi)\n";
    ofs << "tree.X = [ ";
    for (int i=1; i<nodes.size(); i++) {
        ofs << states[nodes[i]->state_id].X(0) << ", " << states[nodes[i]->state_id].X(1) << ", " << states[nodes[i]->state_id].X(2) << ", " << states[nodes[i]->state_id].phi << "; ";
    }
    ofs << "];\n\n";

    ofs << "% Action at parent state : u = (v, w, c, t)\n";
    ofs << "tree.u = [ ";
    for (int i=1; i<nodes.size(); i++) {
        if (nodes[i]->t<=LEADER_N_SAMPLES*TIME_EXECUTE)
            ofs << cnt_N[nodes[i]->control].v << ", " << cnt_N[nodes[i]->control].w << ", " << cnt_N[nodes[i]->control].c << ", " << cnt_N[nodes[i]->control].t << "; ";
        else
            ofs << cnt_M[nodes[i]->control].v << ", " << cnt_M[nodes[i]->control].w << ", " << cnt_M[nodes[i]->control].c << ", " << cnt_M[nodes[i]->control].t << "; ";
    }
    ofs << "];\n\n";
    ofs << "end" << endl;
    ofs.flush();
    ofs.close();
    std::cout << "RRT: Tree file " << filename << " created." << std::endl;
}

void RRT::save_path(const char *filename)
{
    std::ofstream ofs(filename);
    ofs << "function [traj]=rrt()" << endl;
    ofs << "% Quality of the path\n";
    ofs << "traj.quality = " << trajectory_quality << ";\n";
    
    ofs << "% u = (v, w, c, t)\n";
    ofs << "traj.u = [ ";
    for (int i=0; i<out_trajectory.size(); i++) {
        ofs << out_trajectory[i].v << ", " << out_trajectory[i].w << ", " << out_trajectory[i].c << ", " << out_trajectory[i].t << "; ";
    }
    ofs << "];\n\n";
    ofs << "end" << endl;
    ofs.flush();
    ofs.close();
    
    std::cout << "RRT: Trajectory file " << filename << " created." << std::endl;
}
