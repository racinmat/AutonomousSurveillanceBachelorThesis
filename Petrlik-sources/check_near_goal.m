function [ goal_reached ] = check_near_goal( loc, goal_reached )
%CHECK_OBSTACLE Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs goals

for m = 1:length(goals);
    for n = 1:number_of_uavs
        if goals{m}.is_near(loc(:,n));
            goal_reached(1,n) = m;
        end
    end
end
end