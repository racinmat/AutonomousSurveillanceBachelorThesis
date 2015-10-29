function [ collision ] = check_inside_obstacle( s_new )
%CHECK_INSIDE_OBSTACLE Summary of this function goes here
%   Detailed explanation goes here

global obstacles

collision = false;

for m = 1:length(obstacles)
    if any(s_new(1,:) > obstacles{m}.x) && any(s_new(1,:)<obstacles{m}.x+obstacles{m}.width) ...
            && any(s_new(2,:)>obstacles{m}.y) && any(s_new(2,:)<obstacles{m}.y+obstacles{m}.height)
        collision = true;
    end
end

end

