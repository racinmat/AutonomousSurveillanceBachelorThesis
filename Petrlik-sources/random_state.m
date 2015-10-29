function [s_rand] = random_state(x1, x2, y1, y2)
%RANDOM_STATE Generate random state outside of obstacles

s_rand = NaN(2,1);

while true
    s_rand(:,1) = [round(x1+rand()*(x2-x1)); round(y1+rand()*(y2-y1))];
    if ~check_inside_obstacle(s_rand)
        break
    end
end
end