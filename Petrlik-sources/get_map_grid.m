function [ map_grid ] = get_map_grid( goal_index )
%GET_MAP_GRID Summary of this function goes here
%   Detailed explanation goes here

global params

map_grid = NaN(params.world_dimensions(2,1)/params.cell_size, ...
    params.world_dimensions(1,1)/params.cell_size);
for k=1:params.world_dimensions(2,1)/params.cell_size+1
    for m=1:params.world_dimensions(1,1)/params.cell_size+1
        map_grid(k,m) = contains_obj([(m-1)*params.cell_size;(k-1)*params.cell_size], ...
            [(m)*params.cell_size;(k)*params.cell_size], goal_index);
    end
end
end

