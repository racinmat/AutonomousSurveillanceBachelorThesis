function [  ] = visualize_path( path )
%UNTITLED Visualizes the path found by RRT algorithm
%   path - path to be visualized

global number_of_uavs figures colors

set(0, 'CurrentFigure', figures.fig1);

for n = 1:number_of_uavs
    edges(1,n) = animatedline('LineWidth', 2); %#ok
end

path = flip(path);
tpp = path(1);
trajectory = path(1);

for m = 2:length(path)
    [tp, tra] = car_like_motion_model(path(m-1), path(m).prev_inputs);
    trajectory = [trajectory tra]; %#ok
    tpp = [tpp tp]; %#ok
end

for m = 1:length(trajectory)
    for n = 1:number_of_uavs
        addpoints(edges(1,n),trajectory(m).loc(1,n), trajectory(m).loc(2,n))
        set(edges(1,n), 'Color', colors(n,:));
    end
end

