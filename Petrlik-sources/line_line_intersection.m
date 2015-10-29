function [ p ] = line_line_intersection( p1, p2, p3, p4 )
%LINE_LINE_INTERSECTION Summary of this function goes here
%   Detailed explanation goes here

x1 = p1(1,1);
y1 = p1(2,1);
x2 = p2(1,1);
y2 = p2(2,1);
x3 = p3(1,1);
y3 = p3(2,1);
x4 = p4(1,1);
y4 = p4(2,1);

px = ((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/ ...
            ((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));
py = ((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/ ...
            ((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));

p = [px; py];
        
end

