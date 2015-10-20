
global params maps colors figures obstacles goals

figures.fig1 = figure('Name','Planning of swarm deployment for autonomous surveillance'...
    ,'NumberTitle','off',...
    'Visible','off','Units','normalized','position',[0.06 0 0.94 1]);
set(0, 'CurrentFigure', figures.fig1);
colors = [[0 0.4470 0.7410]; [0.8500 0.3250 0.0980]; [0.9290 0.6940 0.1250]; ...
    [0.4940 0.1840 0.5560]; [0.4660 0.6740 0.1880]; ...
    [0.3010 0.7450 0.9330]; [0.6350 0.0780 0.1840]; [255 170 204]./255; [85 255 153]./255; [153 255 85]./255];

prepare_parameters();
params.map=2;
params.world_dimensions = [1000;1000];

prepare_maps();
params.initial_node = maps(1,params.map).init_node;
obstacles = maps(1,params.map).obstacles;
goals = maps(1,params.map).goals;

init_goals();
scene_init();

figures.fig1.Visible = 'on';