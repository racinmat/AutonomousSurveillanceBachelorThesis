clear all
close all

global params maps goals obstacles figures stop colors output

prepare_parameters();

iterations = 100;

params.map = 4;
params.visualize = false;
params.final_path = false;
params.number_of_solutions = 1;
params.max_nodes = 5000;
params.random_init = true;
params.sampling_constraints = false;

prepare_maps();
params.initial_node = maps(1,params.map).init_node;
obstacles = maps(1,params.map).obstacles;
goals = maps(1,params.map).goals;
init_goals();



stop = false;

dir_path = strcat('output/exp_rnd_init/',date, '/', datestr(clock,30));
mkdir(dir_path)

for algorithm=1:2
    params.algorithm = algorithm;
    params.initial_node = maps(1,params.map).init_node;
    obstacles = maps(1,params.map).obstacles;
    goals = maps(1,params.map).goals;
    init_goals();
    
    k=1;
    while k <= iterations
        try
            if params.visualize
                figures.fig1 = figure('Name','Planning of swarm deployment for autonomous surveillance'...
                    ,'NumberTitle','off',...
                    'Visible','off','Units','normalized');
                set(0, 'CurrentFigure', figures.fig1);
                colors = [[0 0.4470 0.7410]; [0.8500 0.3250 0.0980]; [0.9290 0.6940 0.1250]; ...
                    [0.4940 0.1840 0.5560]; [0.4660 0.6740 0.1880]; ...
                    [0.3010 0.7450 0.9330]; [0.6350 0.0780 0.1840]; [255 170 204]./255; [85 255 153]./255; [153 255 85]./255];
                %            scene_init();
            end
            fprintf('________________________________________________________________\n');
            fprintf('Random init experiment algorithm %d, iteration %d starting\n',params.algorithm, k);
            start_from_gui();
            fname = strcat('algo',int2str(params.algorithm), 'iter', int2str(k));
            matfile = fullfile(dir_path, fname);
            data = struct('params', params, 'output', output); %#ok
            save(matfile, 'data');
        catch err
            disp('Iteration failed due to unresolved exception');
            disp(err);
            disp(err.message);
            disp(err.stack);
            disp(err.identifier);
            continue
        end
        k=k+1;
    end
end

%RRT Compact
params.sampling_constraints = true;
params.algorithm = 1;
params.initial_node = maps(1,params.map).init_node;
obstacles = maps(1,params.map).obstacles;
goals = maps(1,params.map).goals;
init_goals();

k=1;
while k <= iterations
    try
        if params.visualize
            figures.fig1 = figure('Name','Planning of swarm deployment for autonomous surveillance'...
                ,'NumberTitle','off',...
                'Visible','off','Units','normalized');
            set(0, 'CurrentFigure', figures.fig1);
            colors = [[0 0.4470 0.7410]; [0.8500 0.3250 0.0980]; [0.9290 0.6940 0.1250]; ...
                [0.4940 0.1840 0.5560]; [0.4660 0.6740 0.1880]; ...
                [0.3010 0.7450 0.9330]; [0.6350 0.0780 0.1840]; [255 170 204]./255; [85 255 153]./255; [153 255 85]./255];
            %            scene_init();
        end
        fprintf('________________________________________________________________\n');
        fprintf('Random init experiment algorithm 4, iteration %d starting\n', k);
        start_from_gui();
        fname = strcat('algo',int2str(4), 'iter', int2str(k));
        matfile = fullfile(dir_path, fname);
        data = struct('params', params, 'output', output); %#ok
        save(matfile, 'data');
    catch err
        disp('Iteration failed due to unresolved exception');
        disp(err);
        disp(err.message);
        disp(err.stack);
        disp(err.identifier);
        continue
    end
    k=k+1;
end