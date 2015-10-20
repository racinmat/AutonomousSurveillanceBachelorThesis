function [  ] = visualize_steps( path )
%VISUALIZE_STEPS Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs figures params colors

uavs = NaN(1,number_of_uavs);
params.fields_of_view = NaN(2,number_of_uavs);
trajectory = NaN(2,number_of_uavs, length(path));
set(0, 'CurrentFigure', figures.fig1);
set(figures.fig1, 'KeyReleaseFcn', @key_release);
hold on
m = 1;

trajectory = path(1:end);
trajectory = flip(trajectory);

    function key_release(~, event)
        if strcmp(event.Key, 'rightarrow')
            m = m + 1;
            if m > length(path)
                m = m -1;
                return
            end
        elseif strcmp(event.Key, 'leftarrow')
            m = m - 1;
            if m < 1
                m = 1;
                return
            end
        else
            return
        end
        for n = 1:number_of_uavs
            if ishandle(uavs(n))
                delete(uavs(n));
            end
            if ishandle(params.fields_of_view(1,n))
                delete(params.fields_of_view(1,n));
            end
            if ishandle(params.fields_of_view(2,n))
                delete(params.fields_of_view(2,n));
            end
            params.fields_of_view(:,n) = [circle( trajectory(m).loc(1,n), trajectory(m).loc(2,n), params.relative_distance.min/2, colors(n,:)); ...
                circle( trajectory(m).loc(1,n), trajectory(m).loc(2,n), params.relative_distance.max, colors(n,:))];
        end
    end
end





