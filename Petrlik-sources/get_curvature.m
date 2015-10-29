function [ curvature ] = get_curvature( path )
%GET_CURVATURE Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs

curvature = 0;
for m=2:length(path)
    for n=1:number_of_uavs
    curvature = curvature + abs(path(m).rot(3,n)-path(m-1).rot(3,n));
    end
end
curvature = curvature/number_of_uavs/length(path);


end

