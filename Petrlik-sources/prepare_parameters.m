function [  ] = prepare_parameters(  )
%PREPARE_PARAMETERS Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs params defaults


params.number_of_uavs = 4;

%Maps defined in prepare_maps.m
params.map = 1;
%Map size
params.world_dimensions = [1000;1000];
%Display debug text
params.debug = false;
%Algorithm: 1-RRT, 2-RRT-Path, 3-PSO
params.algorithm = 2;
%Maximum time of one iteration
params.max_iteration_time = 3600;
%Random initial position od UAVs
params.random_init = false;

%%Visualization parameters
params.wait_for_keypress = 0;
%Visualize tree growth
params.visualize = true;
%Visualize final path
params.final_path = true;
%Draw every n-th node
params.draw_freq = 1;
%Export every figure frame to svg
params.export_svg = false;

%%RRT parameters
%Required number of nodes over AoI
params.number_of_solutions = 10000;
%Minimal number of nodes
params.min_nodes = 1;
%Maximal number of nodes
params.max_nodes = 20000;
%Near nodes to try until a feasible expansion is found
params.near_count = 1000;
%Compact random sample
params.sampling_constraints = true;
%Samples drawn from AoI
params.goal_biased = false;
%Probability to draw a sample from AoI
params.goal_bias = 0.01;
%Nearest neighbor method: 1-sum, 2-max, 3-min
params.nn_method = 1;
%How long after leaving narrow passage is the speed restored
params.exit_np_threshold = 5;

%%Model parameters
%Script test_motion_model for visualizing inputs
%Size of UAV
params.uav_size = 0.5;
%Maximal forward speed input
params.distance_of_new_nodes = 30; %30
%params.max_v = 20;
%params.max_w = pi/2;
%Maximal steering angle input
params.max_turn = pi/200; %pi/120
%Number of steering inputs
params.input_samples_phi = 3;
%Number of forward speed inputs
params.input_samples_dist = 1;
%Add zero input (UAV stays in place)
params.zero_step = false;
%Simulation step length
params.time_step = 0.05;
%Simulation length
params.end_time = 0.5;
%Number of simulation steps
params.simulation_steps = ceil(params.end_time/params.time_step);

%%Localization parameters
%Relative localization on/off
params.relative_localization = true;
%Required neighbors for each UAV only if swarm splitting is enabled
params.required_neighbors = 1;
%Maximal relative distance of UAVs
params.relative_distance.max = 80;
%Minimal relative distance of UAVs
params.relative_distance.min = 5;
%Neighbor must be in certain angle on/off
params.check_fov = false;
%Angle of neighbor localization
params.localization.angle = pi/2;
%Swarm splitting - for more obstacles - true
params.allow_swarm_splitting = false;

%%Cost function parameters
%Camera view rectangle width
params.camera.x = 150;
%Camera view rectangle height
params.camera.y = 100;

%params.near_coef = 0.2;

%%RRT Path
%Old A* - close to obstacles
params.grid_to_nodes_old = false;
%Cell size for A* planning
params.cell_size = 50;
%Area of cell for A* planning
params.cell_area = params.cell_size^2;
%%params.cell_fill = 10*params.uav_size^2;
%Area of cell filled by obstacle to become an obstacle cell
params.cell_fill = params.cell_area/2 + 1;
%Enlarge obstacles to prevent collision
params.inflate_obstacles = 0;
%Sampling method for swarm splitting when multiple AoIs are present
%Details in BP, Default 1
params.rrt_path_sampling_case = 1;
%Radius around the center of the guiding path cell to draw samples from
params.sampling_radius = 60;
%Probabilty to sample around guiding path
params.guided_sampling_prob = 1;
%Distance of UAV from the center of guiding path cell to move to next cell
params.guiding_near_dist = 40;
%Divides speed when at narrow passage
params.np_divisor = 3;

%%PSO
%Final optimization of coverage by PSO
params.pso_optimization = false;
%Number of PSO particles
params.pso_particles = 10;
%Number of PSO iterations
params.pso_iter = 2000;
%PSO best position of one particle constant
params.pso_phi_p = 2;
%PSO best position of all particles constant
params.pso_phi_g = 2.5;
%Starting inertia value
params.pso_inertia_start = 5;   %5
%Final inertia 
params.pso_inertia_end = 1;     %1
%Minimal speed of particle coefficient
%params.pso_vmin = 0.001;
%Maximal speed of particle coefficient
params.pso_vmax = 0.05;

%params.pso_v_div = 5;
%Number of samples of speed of particles
params.pso_v_samples = 7;
%params.pso_time_step = 0.02;
%params.pso_end_time = 5;
%Visualize every PSO particle
params.pso_visualize_all = true;


number_of_uavs = params.number_of_uavs;
defaults = params;
end

