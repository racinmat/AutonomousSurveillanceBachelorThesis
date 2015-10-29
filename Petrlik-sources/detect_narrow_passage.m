function [ ] = detect_narrow_passage( cell )

global params defaults count

if cell.cost>1
    params.distance_of_new_nodes = defaults.distance_of_new_nodes/params.np_divisor;
    params.max_turn = defaults.max_turn*params.np_divisor;
    params.guiding_near_dist = defaults.guiding_near_dist/params.np_divisor;
    count = 0;
   % params.sampling_radius = defaults.sampling_radius/3;
else
    count = count + 1;
    
    %Far from obstacle
    if count > params.exit_np_threshold
    params.distance_of_new_nodes = defaults.distance_of_new_nodes;
    params.max_turn = defaults.max_turn;
    params.guiding_near_dist = defaults.guiding_near_dist;
    count = 0;
    end
   % params.sampling_radius = defaults.sampling_radius;
end

