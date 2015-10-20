function [nodes] = tree_init()

global number_of_uavs params empty_node

number_of_inputs = params.input_samples_phi * params.input_samples_dist;

if params.zero_step
    number_of_inputs = number_of_inputs + 1;
end

empty_node.loc = NaN(3, number_of_uavs);
empty_node.rot = NaN(3, number_of_uavs);
empty_node.v = NaN(3, number_of_uavs);
empty_node.prev = NaN;
empty_node.prev_inputs = NaN(2,number_of_uavs);
empty_node.index = NaN;
empty_node.used_inputs = false(number_of_inputs, number_of_uavs);
empty_node.best_cfp = NaN;

nodes = repmat(empty_node, 1, params.max_nodes);

params.initial_node.v(1:3,1:number_of_uavs) = 0;
params.initial_node.prev = 0;
params.initial_node.prev_inputs = zeros(2,number_of_uavs);
params.initial_node.index = 1;
params.initial_node.used_inputs = false(number_of_inputs, number_of_uavs);
params.initial_node.best_cfp = Inf;

nodes(1) = params.initial_node;
end