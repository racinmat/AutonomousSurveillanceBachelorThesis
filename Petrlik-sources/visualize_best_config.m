function [  ] = visualize_best_config( s_best)
%VISUALIZE_BEST_CONFIG Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs params figures colors

figure(figures.fig1);
hold on

fields_of_view = NaN(2,number_of_uavs);
p = NaN(1,number_of_uavs);

for n = 1:number_of_uavs
    fields_of_view(:,n) = [circle( s_best(1,n), s_best(2,n), params.relative_distance.min/2, colors(n,:)); ...
        circle( s_best(1,n), s_best(2,n), params.relative_distance.max, colors(n,:))];
    p(1,n) = patch([s_best(1,n)-params.camera.x/2, s_best(1,n)+params.camera.x/2, s_best(1,n)+params.camera.x/2, s_best(1,n)-params.camera.x/2], ...
        [s_best(2,n)-params.camera.y/2, s_best(2,n)-params.camera.y/2, s_best(2,n)+params.camera.y/2, s_best(2,n)+params.camera.y/2],colors(n,:));
    set(p(1,n), 'edgecolor', colors(n,:), 'LineWidth', 1, 'FaceColor', colors(n,:), 'FaceAlpha', 0.4);
end
hold off
end

