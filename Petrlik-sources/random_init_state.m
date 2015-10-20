function [ ] = random_init_state( )
%RANDOM_INIT_STATE Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs params

params.initial_node.loc = zeros(3,number_of_uavs);
params.initial_node.rot = zeros(3,number_of_uavs);

border = 100;

center = random_state(border, params.world_dimensions(1,1)-border, border, params.world_dimensions(2,1)-border);

% for n=1:number_of_uavs
%     params.initial_node.loc(1:2,n) = random_state_polar(center, params.relative_distance.min, params.relative_distance.max);
%     params.initial_node.rot(3,n) = rand()*2*pi;
% end




while true
for n=1:number_of_uavs
    while true
        s_rand = random_state_polar(center, params.relative_distance.min, params.relative_distance.max);
        if  ~check_inside_obstacle(s_rand) && check_world_bounds(s_rand)
            params.initial_node.loc(1:2,n) = s_rand;
            break
        end
    end
    
end
if ~any(isnan(check_near_goal(params.initial_node.loc)))
    continue
end
rot = rand()*2*pi;
params.initial_node.rot(3,:) = rot;

if check_localization_sep(params.initial_node)
    break
end
end

end

