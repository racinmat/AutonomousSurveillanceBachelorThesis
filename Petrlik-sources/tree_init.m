function [nodes] = tree_init()

global number_of_uavs params empty_node empty_trajectory

number_of_inputs = (params.input_samples_phi * params.input_samples_dist)^number_of_uavs;

if params.zero_step
    number_of_inputs = number_of_inputs + 1;
end

empty_node.loc = NaN(3, number_of_uavs);
empty_node.rot = NaN(3, number_of_uavs);
empty_node.v = NaN(3, number_of_uavs);
empty_node.prev = NaN;
empty_node.prev_inputs = NaN(2,number_of_uavs);
empty_node.index = NaN;
empty_node.used_inputs = false(number_of_inputs,1);
empty_node.best_cfp = NaN;
empty_node.time_added = NaN;

nodes = repmat(empty_node, 1, params.max_nodes);
empty_trajectory = repmat(empty_node, 1, params.simulation_steps);

params.initial_node.v(1:3,1:number_of_uavs) = 0;
params.initial_node.prev = 0;
params.initial_node.prev_inputs = zeros(2,number_of_uavs);
params.initial_node.index = 1;
params.initial_node.used_inputs = false(number_of_inputs,1);
params.initial_node.best_cfp = Inf;
params.initial_node.time_added = 0;

nodes(1) = params.initial_node;
end