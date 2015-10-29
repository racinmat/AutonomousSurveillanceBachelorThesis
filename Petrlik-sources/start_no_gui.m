
global params maps goals obstacles figures stop colors output

figures.fig1 = figure('Name','Planning of swarm deployment for autonomous surveillance'...
    ,'NumberTitle','off',...
    'Visible','off','Units','normalized');

set(0, 'CurrentFigure', figures.fig1);
colors = [[0 0.4470 0.7410]; [0.8500 0.3250 0.0980]; [0.9290 0.6940 0.1250]; ...
    [0.4940 0.1840 0.5560]; [0.4660 0.6740 0.1880]; ...
    [0.3010 0.7450 0.9330]; [0.6350 0.0780 0.1840]; [255 170 204]./255; [85 255 153]./255; [153 255 85]./255];
stop = false;

%axes = axes('Units','normalized','Position',[0.45 0.05 0.5 0.9]);
set(gca,'xtick',[],'ytick',[])
box on;

prepare_parameters();
prepare_maps();
params.map = 1;
params.algorithm = 1;
params.number_of_solutions = 20;
params.max_nodes = 10000;
params.initial_node = maps(1,params.map).init_node;
obstacles = maps(1,params.map).obstacles;
goals = maps(1,params.map).goals;
init_goals();
scene_init();
start_from_gui();
dir_path = strcat('output/',date);
        mkdir(dir_path)
        matfile = fullfile(dir_path, datestr(clock,30));
        data = struct('params', params, 'output', output); %#ok
        save(matfile, 'data');