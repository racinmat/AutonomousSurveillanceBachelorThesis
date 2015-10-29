function [s_rand] = random_state_goal(goal_index)
%RANDOM_STATE Generate random state outside of obstacles

global goals

s_rand = NaN(2, 1);

x1 = goals{goal_index}.x;
x2 = goals{goal_index}.x + goals{goal_index}.width;
y1 = goals{goal_index}.y;
y2 = goals{goal_index}.y + goals{goal_index}.height;

s_rand(:,1) = random_state(x1, x2, y1, y2);

end