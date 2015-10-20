function [ collision ] = contains_obstacle(p1, p2)

global obstacles

collision = false;
tri_cell1 = [p1(1,1),p1(2,1),0, p2(1,1),p1(2,1),0, p2(1,1),p2(2,1),0];
tri_cell2 = [p1(1,1),p1(2,1),0, p1(1,1),p2(2,1),0, p2(1,1),p2(2,1),0];
zero_trans = [0,0,0, 1,0,0, 0,1,0, 0,0,1];
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
        collision = true;
    end
end
end