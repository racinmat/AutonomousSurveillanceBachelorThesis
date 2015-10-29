function [] = visualize_growth( s_near, s_new, s_rand)
%VISUALIZE_GROWTH Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs params figures colors

uavs = NaN(1:number_of_uavs);
random_states = NaN(number_of_uavs);
edges = NaN(number_of_uavs);
fields_of_view = NaN(2,number_of_uavs);

xlim([0 params.world_dimensions(1,1)]);
ylim([0 params.world_dimensions(2,1)]);

for m = 1:number_of_uavs
    set(0, 'CurrentFigure', figures.fig1);
    hold on
    edges(m) = line([s_near(1,m) s_new(1,m)], [s_near(2,m) s_new(2,m)],'Color', colors(m,:));
    random_states(m) = plot(s_rand(1,m), s_rand(2,m), '+', 'LineWidth', 2);
    set(random_states(m), 'Color', colors(m,:));
    fields_of_view(:,m) = [circle( s_new(1,m), s_new(2,m), params.relative_distance.min/2, colors(m,:)); ...
        circle( s_new(1,m), s_new(2,m), params.relative_distance.max, colors(m,:))];
end

drawnow;
save_video_frame();
if params.wait_for_keypress
    waitforbuttonpress;
end

for m = 1:number_of_uavs
    if ishandle(random_states(m))
        delete(random_states(m));
    end
    if ishandle(uavs(m))
        delete(uavs(m));
    end
    if ishandle(fields_of_view(1,m))
        delete(fields_of_view(1,m));
    end
    if ishandle(fields_of_view(2,m))
        delete(fields_of_view(2,m));
    end
end
end

