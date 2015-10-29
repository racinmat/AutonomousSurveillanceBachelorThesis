function [ intersect ] = line_segments_intersection( p1, p2, p3, p4 )
%LINE_SEGMENTS_INTERSECTION Summary of this function goes here
%   Detailed explanation goes here

intersect = false;

p = line_line_intersection(p1, p2, p3, p4);

if isfinite(p)
    if line_point_intersection(p, p1, p2) && ...
            line_point_intersection(p, p3, p4)
        intersect = true;
    end
end

end

