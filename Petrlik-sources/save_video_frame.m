function [  ] = save_video_frame(  )
%SAVE_VIDEO_FRAME Summary of this function goes here
%   Detailed explanation goes here

global video params

if params.export_svg
video.filename = sprintf('%04d', video.index);
video.path = strcat(video.dir, '/', video.filename);
%print(video.path, '-dpng', '-r100');
print(video.path, '-dsvg');
video.index = video.index + 1;
end

end

