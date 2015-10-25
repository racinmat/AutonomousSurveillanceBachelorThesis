function [ final_nodes ] = rrt_path( guiding_paths )

global params number_of_uavs empty_node nodes goals goal_map stop output

disp('Starting RRT-path...');

% celková dráha všech cest dohromady, poèet nodes ve všech cestách
% dohromady, evidentnì se ta promìnná nikde nepoužívá
gp_length = 0;
for m=1:length(guiding_paths)
    gp_length = gp_length + length(guiding_paths(m));
end
nodes = tree_init();
current_index = [];

for m=1:length(goals)
    current_index = horzcat(current_index, length(guiding_paths{m})); %#ok
end                 % funkce horzcat spojuje matice vedle sebe, pøipojuje matice vpravo, matice msí mít stejný poèet øádkù

assignin('base', 'current_index', current_index);   % promìnná current index je vektor, ve kterém jsou délky cest ke všem AoI
    
current_index = repmat(current_index, number_of_uavs,1);   % matice s délkami cest. sloupec je cesta, øádek je UAV
final_nodes = repmat(empty_node, 1, params.max_nodes);
goal_reached = NaN(1,number_of_uavs);
part_covered = [];

nodes_count = params.max_nodes;
tim = zeros(1,nodes_count-1); %#ok % tahle promìnná se k nièemu nepoužívá

i = 1; % poèet expandovaných nodes
m = 1; % poèet nalezených cest
s = 2; % poèet prùchodù cyklem? prostì se to jen zvìtší o 1 pøi každém prùchodu, nikde se nepoužívá
tstart=tic; % tic je matlabovský timer http://www.mathworks.com/help/matlab/ref/tic.html
    
while (m <= params.number_of_solutions || i < params.min_nodes) && i < params.max_nodes % numver_of_solutions je asi 10 000. 
    if stop
%        final_nodes(m) = new_node;
        break
    end
    tv = tic; % tic je matlabovský timer
    i = i + 1;
    if all(~isnan(goal_reached))    % pokud jsou všechna UAV uvnitø AoI a mají se v rámci AoI rozmístit optimálnì, rrt_path algoritmus už skonèil
        if params.pso_optimization
            [final_node_index, particles] = pso([goals{1}.x goals{1}.x+goals{1}.width; ...
                goals{1}.y goals{1}.y+goals{1}.height], ...
                new_node);
            final_nodes = particles(final_node_index(1), final_node_index(2));
            nodes = nodes(1:(find(isnan([nodes.index]),1)-1));
            nodes = [nodes particles(:,final_node_index(2))']; %#ok
            return
        else
        end
    end
    
    %Random state
    s_rand = random_state_guided(guiding_paths, current_index, goal_reached); % vrátí pole náhodných bodù, jeden pro každou kvadrokoptéru
    
    %Finding appropriate nearest neighbor
    k = 0;
    near_found = false;
    while ~near_found
        if k > params.near_count
            i = i -1;
            disp('Not possible to find near node suitable for expansion');
            break
        end
        [near_node] = nearest_neighbor(s_rand, nodes, k); % near_node v sobì obsahuje souøadnice blízkých nodes pro všechna UAV (tedy 4 rùzné body, pokud mám 4 UAV)
        [near_node, new_node] = select_input(s_rand, near_node);    % zde je motion model, v new_node jsou opìt body pro všechna UAV dosažitelné podle motion modelu. 
        % Vypadá to, že near_node je ve funkci select_input zmìnìná kvùli
        % kontrole pøekážek
        nodes(near_node.index) = near_node; % promìnná nodes je pole, kam se ukládá strom prohledávání u RRT-Path
        if all(near_node.used_inputs) || ...
                any(isnan(new_node.loc(1,:))) || any(isnan(near_node.loc(1,:)))
            k = k + 1;
            check_expandability();
            continue
        end
        near_found = true;
    end   
    if any(isnan(new_node.loc(1,:)))
        check_expandability();
        disp('NaN in new node');
        final_nodes(m) = nodes(i-1);
        break
    end
    
    new_node.index = i;
    new_node.time_added = toc(tstart);
    if params.debug
        fprintf('[debug] Added node index: %d\n', new_node.index);
    end
    
    if mod(i,200)==0
        fprintf('RRT size: %d\n',i);
    end
    
    nodes(i) = new_node;
    s = s + 1;
    
    current_index = guiding_point_reached(new_node, guiding_paths, current_index); % zde se uloží do current_index, kolik nodes zbývá danému UAV do cíle
    goal_reached = check_near_goal(new_node.loc);
    if all(~isnan(goal_reached)) % pokud je nalezen cíl
        output.goal_reached = goal_reached;
        final_nodes(m) = new_node;
        fprintf('%d viable paths found so far.\n',m);
        part_covered(1,end+1) = goal_map.get_area_covered(); %#ok
        goal_map.reset();
        m = m + 1;
    end
    output.distance_of_new_nodes(1,i) = params.distance_of_new_nodes;
%     dt = toc(tv);
%     output.time_to_find_new_node(1,i) = dt;
%     output.iterations(1,i) = i;
%     try
%         output.time_running(1,i) = output.time_running(1,i-1) + dt;
%     catch
%         output.time_running(1,i) = dt;
%     end
%     if output.time_running(1,i) > params.max_iteration_time
%         final_nodes(m) = new_node;
%         return
%     end
%     output.goal_reached = goal_reached;
    
    %Visualize
    if params.visualize ==true && mod(i,params.draw_freq) == 0
        visualize_growth(near_node.loc, new_node.loc, s_rand);
    end
    
end
final_nodes(m) = nodes(i);
goal_reached = check_near_goal(new_node.loc);
output.goal_reached = goal_reached;
final_nodes = final_nodes(1:(find(isnan([final_nodes.index]),1)-1));
if any(isnan([nodes.index]))
nodes = nodes(1:(find(isnan([nodes.index]),1)-1));
end
output.nodes = nodes;
disp('RRT-Path finished');
toc(tstart); % matlabovský timer
end