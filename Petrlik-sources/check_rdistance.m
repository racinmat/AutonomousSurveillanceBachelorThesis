function [ in_range ] = check_rdistance( s_new)
%CHECK_OBSTACLE Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs params
in_range = false;
count = 0;

for m = 1:number_of_uavs
    for n = 1:number_of_uavs
        if n ~= m
            if norm([s_new(1,m)-s_new(1,n) s_new(2,m)-s_new(2,n)]) > params.relative_distance.min ...
                    && norm([s_new(1,m)-s_new(1,n) s_new(2,m)-s_new(2,n)]) < params.relative_distance.max
                count = count + 1;
                break
            end
        end
    end
end
if count >= number_of_uavs
    in_range = true;
    
end