close all
clear all

%Change to directory containing results
dir_path = 'output/exp_np_w/13-Jul-2015/20150713T201521/';

iterations = 3;
%width = [10 20 30 40 50 100 150 200];
width = [30 40 50 100 150 200];

index = 1;
for map=73:78
    sum.path_length(index,1) = 0;
    sum.iterations(index,1) = 0;
    sum.runtime(index,1) = 0;
    for iter=1:iterations
        load(strcat(dir_path, 'map', int2str(map), 'iter', int2str(iter), '.mat'));
        results(index, iter) = data;
        sum.path_length = sum.path_length + length(results(index,iter).output.path);
        sum.iterations = sum.iterations + length(results(index,iter).output.nodes);
        sum.runtime = sum.runtime + results(index,iter).output.nodes(end).time_added;
    end
    results_mean(index,1).path_length = sum.path_length(index,1)/iterations;
    results_mean(index,1).iterations = sum.iterations(index,1)/iterations;
    results_mean(index,1).runtime = sum.runtime(index,1)/iterations;
    index = index + 1;
end

figure_name = sprintf('Narrow passage width experiment');

%% Plotting
figure('Name', figure_name, 'NumberTitle','off');
subplot(2,2,1);
bar(width, [results_mean(:,1).iterations]);
xlabel('Width');
ylabel('Iterations');
title('Iterations');

subplot(2,2,2);
bar(width, [results_mean(:,1).path_length]);
xlabel('Width');
ylabel('Path length');
title('Path length');

subplot(2,2,3);
bar(width, [results_mean(:,1).runtime]);
xlabel('Width');
ylabel('Runtime');
title('Runtime');


