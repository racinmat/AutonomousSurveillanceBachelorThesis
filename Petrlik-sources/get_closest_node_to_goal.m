function [ best_node, lowest_distance ] = get_closest_node_to_goal(  )
%GET_CLOSEST_NODE_TO_GOAL Summary of this function goes here
%   Detailed explanation goes here

global nodes number_of_uavs goals

lowest_distance = inf(length(goals),number_of_uavs);
best_node = nodes(1);
for m=1:length(nodes)
    for k=1:length(goals)
        for n=1:number_of_uavs
        distance(k,n) = sqrt((nodes(m).loc(1,n)-(goals{k}.x+goals{k}.width/2))^2 + ...
            (nodes(m).loc(2,n)-(goals{k}.y+goals{k}.height/2))^2);
        end
    end
    if sum(sum(distance))<sum(sum(lowest_distance))
        lowest_distance = distance;
        best_node=nodes(m);
    end
end
end

