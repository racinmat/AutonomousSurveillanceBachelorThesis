function [ intersect ] = line_point_intersection( q, p1, p2 )
%LINE_POINT_INTERSECTION Summary of this function goes here
%   Detailed explanation goes here

tolerance = 1e-10;

x = q(1,1);
y = q(2,1);
x1 = p1(1,1);
y1 = p1(2,1);
x2 = p2(1,1);
y2 = p2(2,1);

value = ((x-x1)^2+(y-y1)^2) + ((x-x2)^2+(y-y2)^2) - ((x1-x2)^2+(y1-y2)^2);

intersect = ~(value > tolerance);

end

