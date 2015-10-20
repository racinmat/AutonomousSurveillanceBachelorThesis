function [] = scene_init()
%SCENE_INIT Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs params figures goals obstacles colors

figure(figures.fig1);

hold off

for m = 1:length(goals)
    goals{m}.draw;
    hold on
end

axis([0 params.world_dimensions(1,1) 0 params.world_dimensions(2,1)]);
axis square

for m = 1:length(obstacles)
    obstacles{m}.draw;
end

for n = 1:number_of_uavs
    plot(params.initial_node.loc(1,n), params.initial_node.loc(2,n), 'x',...
        'Color', colors(n,:), 'LineWidth', 1);
end

