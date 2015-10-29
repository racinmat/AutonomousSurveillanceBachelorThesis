function [ fitness, map ] = fitness_function( final_node )
%FITNESS_FUNCTION Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs params goal_map

for n = 1:number_of_uavs
    goal_map.apply_camera(final_node(1,n), final_node(2,n), params.camera.x, params.camera.y, params.world_dimensions);
end
fitness = goal_map.get_sum();
map = goal_map;
end

