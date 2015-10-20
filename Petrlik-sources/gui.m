function gui

global figures colors params obstacles goals stop maps output number_of_uavs
prepare_parameters();
prepare_maps();

figures.fig1 = figure('Name','Planning of swarm deployment for autonomous surveillance'...
    ,'NumberTitle','off',...
    'Visible','off','Units','normalized','position',[0.06 0 0.94 1]);
set(0, 'CurrentFigure', figures.fig1);
colors = [[0 0.4470 0.7410]; [0.8500 0.3250 0.0980]; [0.9290 0.6940 0.1250]; ...
    [0.4940 0.1840 0.5560]; [0.4660 0.6740 0.1880]; ...
    [0.3010 0.7450 0.9330]; [0.6350 0.0780 0.1840]; [255 170 204]./255; [85 255 153]./255; [153 255 85]./255];

ui.text_algorithm = uicontrol('Style','text','String','Select algorithm',...
    'Units','normalized','Position',[-0.03 0.95 0.15 0.02]);
ui.popup_algorithm = uicontrol('Style','popupmenu',...
    'String',{'RRT','RRT-Path','PSO'},...
    'Value',params.algorithm,...
    'Units','normalized',...
    'Position',[0.02 0.92 0.06 0.02],...
    'Callback',@popup_algorithm_Callback);

ui.text_numberofuavs = uicontrol('Style','text','String','Number of UAVs',...
    'Units','normalized','Position',[0.15 0.95 0.15 0.02]);
ui.popup_numberofuavs = uicontrol('Style','popupmenu',...
    'String',{'2','4','6','8','10'},...
    'Value',params.number_of_uavs/2,...
    'Units','normalized',...
    'Position',[0.2 0.92 0.06 0.02],...
    'Callback',@popup_numberofuavs_Callback);

ui.text_select_map = uicontrol('Style','text','String','Select map',...
    'Units','normalized','Position',[0.34 0.95 0.05 0.02]);
ui.popup_map = uicontrol('Style','popupmenu',...
    'String',{'Map 1','Map 2','Map 3','Map 4','Map 5','Map 6'},...
    'Units','normalized',...
    'Position',[0.35 0.92 0.05 0.02],...
    'Callback',@popup_map_Callback);
ui.button_start = uicontrol('Style','pushbutton','String','Start',...
    'Units','normalized','Position',[0.30 0.1 0.05 0.05],...
    'Callback',@button_start_Callback);
ui.button_stop = uicontrol('Style','pushbutton','String','Stop',...
    'Units','normalized','Position',[0.36 0.1 0.05 0.05],...
    'Enable', 'off', 'Callback',@button_stop_Callback);

ui.axes = axes('Units','normalized','Position',[0.45 0.05 0.5 0.9]);
set(gca,'xtick',[],'ytick',[])
box on;

params.initial_node = maps(1,params.map).init_node;
obstacles = maps(1,params.map).obstacles;
goals = maps(1,params.map).goals;
init_goals();
scene_init();

%Make the UI visible.
figures.fig1.Visible = 'on';

    function popup_algorithm_Callback(source,~)
        val = source.Value;
        params.algorithm = val;
    end

    function popup_numberofuavs_Callback(source,~)
        val = source.Value;
        params.number_of_uavs = val*2;
        number_of_uavs = val*2;
        maps = [];
        prepare_maps();
        params.initial_node = maps(params.map).init_node;
        cla
        init_goals();
        scene_init();
    end

    function popup_map_Callback(source,~)
        cla
        str = source.String;
        val = source.Value;
        params.map = val;
        
        switch str{val};
            case 'Map 1'
                params.initial_node = maps(1,1).init_node;
                obstacles = maps(1,1).obstacles;
                goals = maps(1,1).goals;
                
            case 'Map 2'
                params.initial_node = maps(1,2).init_node;
                obstacles = maps(1,2).obstacles;
                goals = maps(1,2).goals;
                
            case 'Map 3'
                params.initial_node = maps(1,3).init_node;
                obstacles = maps(1,3).obstacles;
                goals = maps(1,3).goals;
                
            case 'Map 4'
                params.initial_node = maps(1,4).init_node;
                obstacles = maps(1,4).obstacles;
                goals = maps(1,4).goals;
                
            case 'Map 5'
                params.initial_node = maps(1,5).init_node;
                obstacles = maps(1,5).obstacles;
                goals = maps(1,5).goals;
                
            case 'Map 6'
                params.initial_node = maps(1,6).init_node;
                obstacles = maps(1,6).obstacles;
                goals = maps(1,6).goals;
                
            case 'Map 7'
                params.initial_node = maps(1,7).init_node;
                obstacles = maps(1,7).obstacles;
                goals = maps(1,7).goals;
        end
        
        init_goals();
        scene_init();
    end

    function button_start_Callback(~,~)
        ui.button_start.Enable = 'off';
        ui.popup_map.Enable = 'off';
        ui.popup_algorithm.Enable = 'off';
        ui.popup_numberofuavs.Enable = 'off';
        ui.button_stop.Enable = 'on';
        stop = false;
        cla
        scene_init();
        start_from_gui();
    end

    function button_stop_Callback(~,~)      
        stop = true;
        drawnow
        hold off
        ui.button_stop.Enable = 'off';
        ui.button_start.Enable = 'on';
        ui.popup_map.Enable = 'on';
        ui.popup_algorithm.Enable = 'on';
        ui.popup_numberofuavs.Enable = 'on';
        stop = true;        
        save_output();
    end

    function save_output()
        dir_path = strcat('output/',date);
        mkdir(dir_path)
        matfile = fullfile(dir_path, datestr(clock,30));
        data = struct('params', params, 'output', output); %#ok
        save(matfile, 'data');
    end
end