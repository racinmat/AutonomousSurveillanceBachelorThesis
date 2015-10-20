function [  ] = save_output(  )
%SAVE_OUTPUT Summary of this function goes here
%   Detailed explanation goes here

global params output

        dir_path = strcat('output/',date);
        mkdir(dir_path)
        matfile = fullfile(dir_path, datestr(clock,30));
        data = struct('params', params, 'output', output); %#ok
        save(matfile, 'data');
end

