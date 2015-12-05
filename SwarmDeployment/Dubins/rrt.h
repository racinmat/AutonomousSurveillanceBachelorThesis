#ifndef RRT_H
#define RRT_H

#include "../types.h"
#include "../world.h"
#include "MPNN/include/DNN/multiann.h"
#include "MPNN/include/DNN/ANN.h"
#include "Dubins/geom/geom.h"


namespace imr {
namespace rrt_algorithm {
namespace rrt{

class RRT
{
    struct node;
    struct input_seed;
private:
    MPNN::MultiANN<int> *kdTree;
    geom::Dubins *dubins;
    std::vector<node*> nodes;
    Controls cnt_N, cnt_M, out_trajectory;
    States states;
    const double time;

    std::vector<size_t> trajectory;
    double trajectory_quality;

    Vector sample_target();
    node* extend(const size_t nearest, const Vector &target, bool *status);

    void generate_input_seed();
    void construct_tree();
    void find_trajectory();
    Controls construct_trajectory();
    void create_KDTree();
    void add_point_to_KDTree(const State &_state, const int index);
    int find_NN_in_KDTree(const Vector _point);

public:
    RRT(const State &_state, const double _time);
    ~RRT();
    void save_tree(const char *filename);
    void save_path(const char *filename);
    void clear_tree();
    double get_trajectory_quality() const { return trajectory_quality; }
    Controls get_trajectory() const {return out_trajectory;}
    Controls get_new_trajectory();
    void optimize_trajectory();
    void simplify_trajectory();

};

struct RRT::node {
    size_t state_id, control;
    double t;
    vector<size_t> children;

    node() : state_id(-1), control(-1), t(0){}
    node(const size_t _state_id,const size_t _control, const double _t)
        : state_id(_state_id), control(_control), t(_t){}
};

}
}
}
#endif
