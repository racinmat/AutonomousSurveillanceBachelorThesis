clear all
close all

global params maps goals obstacles figures stop colors output

prepare_parameters();

iterations = 3;

params.map = 1;
params.visualize = false;
params.final_path = false;
params.number_of_solutions = 1;
params.max_nodes = 2000;
params.nn_method = 1;

prepare_maps();
params.initial_node = maps(1,params.map).init_node;
obstacles = maps(1,params.map).obstacles;
goals = maps(1,params.map).goals;
init_goals();


if params.visualize
    figures.fig1 = figure('Name','Planning of swarm deployment for autonomous surveillance'...
        ,'NumberTitle','off',...
        'Visible','off','Units','normalized');
    set(0, 'CurrentFigure', figures.fig1);
    colors = [[0 0.4470 0.7410]; [0.8500 0.3250 0.0980]; [0.9290 0.6940 0.1250]; ...
        [0.4940 0.1840 0.5560]; [0.4660 0.6740 0.1880]; ...
        [0.3010 0.7450 0.9330]; [0.6350 0.0780 0.1840]; [255 170 204]./255; [85 255 153]./255; [153 255 85]./255];
    scene_init();
end
stop = false;

dir_path = strcat('output/exp_gp/',date, '/', datestr(clock,30));
mkdir(dir_path)

params.grid_to_nodes_old = true;
    for k=1:iterations
        fprintf('________________________________________________________________\n');
        fprintf('Guiding path experiment old method, iteration %d starting\n', k);
             
        start_from_gui();
        fname = strcat('old_', 'iter', int2str(k));
        matfile = fullfile(dir_path, fname);
        data = struct('params', params, 'output', output); %#ok
        save(matfile, 'data');
    end
    
    params.grid_to_nodes_old = false;
    for k=1:iterations
        fprintf('________________________________________________________________\n');
        fprintf('Guiding path experiment new method, iteration %d starting\n', k);         
        start_from_gui();
        fname = strcat('new_', 'iter', int2str(k));
        matfile = fullfile(dir_path, fname);
        data = struct('params', params, 'output', output); %#ok
        save(matfile, 'data');
    end