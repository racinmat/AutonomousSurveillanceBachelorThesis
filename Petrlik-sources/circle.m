function [ circ ] = circle(Cx, Cy, r, color)
%CIRCLE Summary of this function goes here
%   Detailed explanation goes here

if isnan(color)
    color = 'b';
end

angle = 0:0.01:2*pi;
x = r*cos(angle);
y = r*sin(angle);
circ = plot(Cx+x, Cy+y, 'Color', color);
set(circ, 'LineWidth', 1);
hold on;
end

