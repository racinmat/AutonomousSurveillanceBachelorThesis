function [ collision ] = contains_obj(p1, p2, goal_index)

global params obstacles goals

collision = 0;
cell.x = p1(1,1);
cell.y = p1(2,1);
cell.width = p2(1,1) - p1(1,1);
cell.height = p2(2,1) - p1(2,1);
tri_cell1 = [p1(1,1),p1(2,1),0, p2(1,1),p1(2,1),0, p2(1,1),p2(2,1),0];
tri_cell2 = [p1(1,1),p1(2,1),0, p1(1,1),p2(2,1),0, p2(1,1),p2(2,1),0];
zero_trans = [0,0,0, 1,0,0, 0,1,0, 0,0,1];

%Test if cell contains UAV
col = 0;
for m = 1:size(params.initial_node,2)
    uav = params.initial_node.loc(:,m);
    p1 = [uav(1,1)-params.uav_size/2; uav(2,1)-params.uav_size/2; uav(3,1)];
    p2 = [uav(1,1)+params.uav_size/2; uav(2,1)-params.uav_size/2; uav(3,1)];
    p3 = [uav(1,1)+params.uav_size/2; uav(2,1)+params.uav_size/2; uav(3,1)];
    p4 = [uav(1,1)-params.uav_size/2; uav(2,1)+params.uav_size/2; uav(3,1)];
    
    tri1_uav = [p1(1),p1(2),p1(3), p2(1),p2(2),p2(3), p3(1),p3(2),p3(3)];
    tri2_uav = [p1(1),p1(2),p1(3), p4(1),p4(2),p4(3), p3(1),p3(2),p3(3)];
    col = col + coldetect(tri_cell1, tri1_uav, zero_trans, zero_trans);
    col = col + coldetect(tri_cell1, tri2_uav, zero_trans, zero_trans);
    col = col + coldetect(tri_cell2, tri1_uav, zero_trans, zero_trans);
    col = col + coldetect(tri_cell2, tri2_uav, zero_trans, zero_trans);
    if col > 0
        collision = 2;
    end
end

%Test if cell contains goal
col = 0;
m = goal_index;
obs = goals{m};
p1 = [obs.x; obs.y; 0];
p2 = [obs.x + obs.width; obs.y; 0];
p3 = [obs.x + obs.width; obs.y + obs.height; 0];
p4 = [obs.x; obs.y + obs.height; 0];

tri1_obs = [p1(1),p1(2),p1(3), p2(1),p2(2),p2(3), p3(1),p3(2),p3(3)];
tri2_obs = [p1(1),p1(2),p1(3), p4(1),p4(2),p4(3), p3(1),p3(2),p3(3)];
col = col + coldetect(tri_cell1, tri1_obs, zero_trans, zero_trans);
col = col + coldetect(tri_cell1, tri2_obs, zero_trans, zero_trans);
col = col + coldetect(tri_cell2, tri1_obs, zero_trans, zero_trans);
col = col + coldetect(tri_cell2, tri2_obs, zero_trans, zero_trans);
if col > 0
    collision = 3;
end

%Test if cell contains obstacle
col = 0;
for m = 1:length(obstacles)
    obs = obstacles{m};
    p1 = [obs.x; obs.y; 0];
    p2 = [obs.x + obs.width; obs.y; 0];
    p3 = [obs.x + obs.width; obs.y + obs.height; 0];
    p4 = [obs.x; obs.y + obs.height; 0];
    
    tri1_obs = [p1(1),p1(2),p1(3), p2(1),p2(2),p2(3), p3(1),p3(2),p3(3)];
    tri2_obs = [p1(1),p1(2),p1(3), p4(1),p4(2),p4(3), p3(1),p3(2),p3(3)];
    col = col + coldetect(tri_cell1, tri1_obs, zero_trans, zero_trans);
    col = col + coldetect(tri_cell1, tri2_obs, zero_trans, zero_trans);
    col = col + coldetect(tri_cell2, tri1_obs, zero_trans, zero_trans);
    col = col + coldetect(tri_cell2, tri2_obs, zero_trans, zero_trans);
    if col > 0
        intersect_area = rectint([obs.x obs.y obs.width obs.height], ...
            [cell.x cell.y cell.width cell.height]);
        if  params.cell_area - intersect_area < params.cell_fill
        collision = 1;
        end
    end
end
end