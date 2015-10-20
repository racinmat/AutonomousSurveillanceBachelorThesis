function [ near_node, collision,  uavs_colliding] = check_obstacle_vcollide( near_node, inputs)
%CHECK_OBSTACLE Summary of this function goes here
%   Detailed explanation goes here

global params number_of_uavs obstacles

uavs_colliding = false(1, number_of_uavs);
collision = false;

tri_uav = zeros(number_of_uavs, 9);
tri1_obs = zeros(number_of_uavs, 9);
tri2_obs = zeros(number_of_uavs, 9);

for n = 1:number_of_uavs
    x = near_node.loc(1,n);
    y = near_node.loc(2,n);
    x1 = x-params.uav_size/2;
    y1 = y-params.uav_size/2;
    z1 = 1;
    x2 = x + params.uav_size/2;
    y2 = y - params.uav_size/2;
    z2 = 1;
    x3 = x;
    y3 = y + params.uav_size/2;
    z3 = 1;
    tri_uav(n,:) = [x1,y1,z1, x2,y2,z2, x3,y3,z3];
end

zero_trans = [0,0,0, 1,0,0, 0,1,0, 0,0,1];

for m = 1:length(obstacles)
    obs = obstacles{m};
    p1 = [obs.x; obs.y; 1];
    p2 = [obs.x + obs.width; obs.y; 1];
    p3 = [obs.x + obs.width; obs.y + obs.height; 1];
    p4 = [obs.x; obs.y + obs.height; 1];
    
    tri1_obs(m,:) = [p1(1),p1(2),p1(3), p2(1),p2(2),p2(3), p3(1),p3(2),p3(3)];
    tri2_obs(m,:) = [p1(1),p1(2),p1(3), p4(1),p4(2),p4(3), p3(1),p3(2),p3(3)];
end

for n = 1:number_of_uavs
    for k = 1:size(inputs,3)
        for m = 1:length(obstacles)
            trans = [inputs(1,n,k),inputs(2,n,k),0, 1,0,0, 0,1,0, 0,0,1];
            col = coldetect(tri_uav(n,:), tri1_obs(m,:), trans, zero_trans);
            col = col + coldetect(tri_uav(n,:), tri2_obs(m,:), trans, zero_trans);
            if col > 0
                near_node.used_inputs(k,n) = true;
                collision= true;
                uavs_colliding(1,n) = true;
            end
        end
    end
end

%memory leak in mex file
clear mex
end
