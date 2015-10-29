function [s_rand] = random_state_polar(center, radius_min, radius_max)
%RANDOM_STATE Generate random state outside of obstacles

s_rand = NaN(2,1);

while true
    phi = rand()*2*pi;
    r = radius_min + rand()*(radius_max-radius_min);
    s_rand(:,1) = [center(1,1)+r*cos(phi); center(2,1)+r*sin(phi)];
    if check_world_bounds(s_rand(:,1))
        break
    end
end
end