function [ ] = init_goals( )
%ADD_GOALS Summary of this function goes here
%   Detailed explanation goes here

global params goals goal_map

goal_map = Goal_map(params.world_dimensions(1,1), params.world_dimensions(2,1), 1);
for m = 1:length(goals)
    goal_map.add_goal(goals{m}.x, goals{m}.y, goals{m}.width, goals{m}.height, 100);
end
end

