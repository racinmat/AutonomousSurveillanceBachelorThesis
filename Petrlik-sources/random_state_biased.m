function [s_rand] = random_state_biased( goal_reached )
%RANDOM_STATE Generate random state outside of obstacles

global number_of_uavs params goals

s_rand = NaN(2, number_of_uavs);
g_length = length(goals);

x1 = 0;
x2 = params.world_dimensions(1,1);
y1 = 0;
y2 = params.world_dimensions(2,1);

for n = 1:number_of_uavs
    if ~isnan(goal_reached(1,n))
        s_rand(:,n) = random_state_goal(goal_reached(1,n));
    else
        s_rand(:,n) = random_state(x1, x2, y1, y2);
        if params.goal_biased
            rn = rand(1);
            if rn <= params.goal_bias
                step = params.goal_bias/g_length;
                m = 1;
                while m < g_length
                    if rn <= m*step
                        break
                    end
                    m = m + 1;
                end
                goal = goals{m};
                x1 = goal.x;
                x2 = x1 + goal.width;
                y1 = goal.y;
                y2 = y1 + goal.height;
                s_rand(:,n) = random_state(x1, x2, y1, y2);
            end
        end
    end
end
end