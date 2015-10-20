function [  ] = get_goal_distances( guiding_paths )
%GET_GOAL_DISTANCES Summary of this function goes here
%   Detailed explanation goes here

global params goals output


for m=1:length(goals)
    goal_center = [goals{m}.x+goals{m}.width/2; goals{m}.y+goals{m}.height/2];
output.start_goal_distance.euclidean(1,m) = ...
    sqrt((goal_center(1,1)-params.initial_node.loc(1,1))^2 ...
    + (goal_center(2,1)-params.initial_node.loc(2,1))^2);

guiding_path = guiding_paths{m};
output.start_goal_distance.a_star(1,m) = 0;
for k=2:length(guiding_path)
output.start_goal_distance.a_star(1,m) = ...
    output.start_goal_distance.a_star(1,m) ...
    + sqrt((guiding_path(k).x - guiding_path(k-1).x)^2 ...
    + (guiding_path(k).y - guiding_path(k-1).y)^2);
end
end

end

