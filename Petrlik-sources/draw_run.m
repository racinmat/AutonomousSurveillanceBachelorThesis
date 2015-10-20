close all
clear all %#ok
clc

global figures colors params obstacles goals maps number_of_uavs

figures.fig1 = figure('Name','Planning of swarm deployment for autonomous surveillance'...
    ,'NumberTitle','off',...
    'Visible','off','Units','normalized','position',[0.06 0 0.94 1]);
set(0, 'CurrentFigure', figures.fig1);
colors = [[0 0.4470 0.7410]; [0.8500 0.3250 0.0980]; [0.9290 0.6940 0.1250]; ...
    [0.4940 0.1840 0.5560]; [0.4660 0.6740 0.1880]; ...
    [0.3010 0.7450 0.9330]; [0.6350 0.0780 0.1840]; [255 170 204]./255; [85 255 153]./255; [153 255 85]./255];

prepare_parameters();
params.number_of_uavs = 4;
number_of_uavs = 4;
params.map = 1;
params.algorithm = 3;
params.visualize = true;

prepare_maps();
params.initial_node = maps(1,params.map).init_node;
obstacles = maps(1,params.map).obstacles;
goals = maps(1,params.map).goals;

init_goals();
scene_init();

%Make the UI visible.
figures.fig1.Visible = 'on';
init_goals();
scene_init();
start_from_gui();