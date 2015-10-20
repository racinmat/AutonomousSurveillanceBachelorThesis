function [  ] = prepare_parameters(  )
%PREPARE_PARAMETERS Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs params


params.number_of_uavs = 4;
params.map = 1;
params.world_dimensions = [1000;1000];
params.debug = false;
params.algorithm = 2;
params.max_iteration_time = 3600;

%Visualization parameters
params.wait_for_keypress = 0;
params.visualize = true;
params.draw_freq = 1;

%RRT parameters
params.number_of_solutions = 10000;
params.min_nodes = 1;
params.max_nodes = 20000;
params.near_count = 50;
params.sampling_constraints = false;
params.goal_biased = false;
params.goal_bias = 0.01;

%Model parameters
params.uav_size = 0.5;
params.distance_of_new_nodes = 30;
params.max_v = 20;
params.max_w = pi/2;
params.max_turn = pi/30;
params.input_samples_phi = 9;
params.input_samples_dist = 3;
params.zero_step = false;

%Localization parameters
params.relative_localization = true;
params.required_neighbors = 1;
params.relative_distance.max = 80;
params.relative_distance.min = 5;
params.check_fov = false;
params.localization.angle = pi/2;

%Cost function parameters
params.camera.x = 150;
params.camera.y = 100;
params.near_coef = 0.2;

%RRT Path
params.cell_size = 50;
params.inflate_obstacles = 0;
params.rrt_path_sampling_case = 1;
params.sampling_radius = 60;
params.guided_sampling_prob = 0.80;
params.guiding_near_dist = 40;

%PSO
params.pso_optimization = false;
params.pso_particles = 10;
params.pso_iter = 2000;
params.pso_phi_p = 2;
params.pso_phi_g = 2.5;
params.pso_inertia_start = 5;
params.pso_inertia_end = 1;
params.pso_vmin = 0.001;
params.pso_vmax = 0.05;
params.pso_v_div = 5;
params.pso_v_samples = 7;
params.pso_time_step = 0.02;
params.pso_end_time = 5;

number_of_uavs = params.number_of_uavs;
end

