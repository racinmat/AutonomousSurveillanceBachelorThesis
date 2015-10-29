function [ s_rand ] = random_state_constraints( x1, x2, y1, y2, goal_reached )
%RANDOM_STATE_CONSTRAINTS Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs params

s_rand = NaN(2,number_of_uavs);

% n = number_of_uavs;
% while n>0
%     s_rand(:,n) = [round(x1+rand()*(x2-x1)); round(y1+rand()*(y2-y1))];
%     s_rand(:,n-1) = random_state_polar([s_rand(1,n);s_rand(2,n)], ...
%         params.relative_distance.min, params.relative_distance.max);
%     n = n-2;
%     if mod(n,2)~=0
%         s_rand(:,n-2) = random_state_polar([s_rand(1,n);s_rand(2,n)], ...
%             params.relative_distance.min, params.relative_distance.max);
%         n = n-1;
%     end
% end

center = [round(x1+rand()*(x2-x1)); round(y1+rand()*(y2-y1))];
for n=1:number_of_uavs
    if goal_reached(1,n) > 0
        s_rand(:,n) = random_state_goal(goal_reached(1,n));
    else
        s_rand(:,n) = random_state_polar(center,0,params.relative_distance.max*3);
    end
end
