function [ in_bounds ] = check_world_bounds( node )
%CHECK_WORLD_BOUNDS Summary of this function goes here
%   Detailed explanation goes here

global params

in_bounds = false;

if node(1,1)<params.world_dimensions(1,1) && ...
        node(1,1)>0 && ...
        node(2,1)<params.world_dimensions(2,1) && ...
        node(2,1)>0
    in_bounds = true;
end
end

