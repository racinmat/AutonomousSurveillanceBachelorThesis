function [ intersect ] = trajectory_intersection( near_node, tmp_node )
%TRAJECTORY_INTERSECTION Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs

intersect = false;
for n=1:number_of_uavs
    for m=1:number_of_uavs
        if m~=n && line_segments_intersection(near_node.loc(:,n), tmp_node.loc(:,n), ...
                near_node.loc(:,m), tmp_node.loc(:,m))
            intersect = true;
            break
        end
    end
end

end

