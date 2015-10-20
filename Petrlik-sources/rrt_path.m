function [ final_nodes, output ] = rrt_path( guiding_paths )

global params number_of_uavs empty_node nodes goals goal_map stop

disp('Starting RRT-path...');

gp_length = 0;
for m=1:length(guiding_paths)
    gp_length = gp_length + length(guiding_paths(m));
end
nodes = tree_init();
current_index = [];

for m=1:length(goals)
    current_index = horzcat(current_index, length(guiding_paths{m})); %#ok
end

current_index = repmat(current_index, number_of_uavs,1);
final_nodes = repmat(empty_node, 1, params.max_nodes);
goal_reached = NaN(1,number_of_uavs);
part_covered = [];

nodes_count = params.max_nodes;
tim = zeros(1,nodes_count-1); %#ok

i = 1;
m = 1;
s = 2;
tstart=tic;
while (m <= params.number_of_solutions || i < params.min_nodes) && i < params.max_nodes
    if stop
        final_nodes(m) = new_node;
        break
    end
    tv = tic;
    i = i + 1;
    if all(~isnan(goal_reached))
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
    s_rand = random_state_guided(guiding_paths, current_index, goal_reached);
    
    %Finding appropriate nearest neighbor
    k = 0;
    near_found = false;
    while ~near_found
        if k > params.near_count
            i = i -1;
            disp('Not possible to find near node suitable for expansion');
            break
        end
        [near_node]= nearest_neighbor(s_rand, nodes, k);
        [near_node, new_node] = select_input(s_rand, near_node);
        nodes(near_node.index) = near_node;
        if any(isnan(new_node.loc(1,:))) || any(isnan(near_node.loc(1,:)))
            k = k + 1;
            check_expandability();
            continue
        end
        near_found = true;
    end   
    
    new_node.index = i;
    if params.debug
        fprintf('[debug] Added node index: %d\n', new_node.index);
    end
    
    nodes(i) = new_node;
    s = s + 1;
    
    current_index = guiding_point_reached(new_node, guiding_paths, current_index);
    goal_reached = check_near_goal(new_node.loc, goal_reached);
    if all(~isnan(goal_reached))
        output.goal_reached = goal_reached;
        final_nodes(m) = new_node;
        fprintf('%d viable paths found so far.\n',m);
        part_covered(1,end+1) = goal_map.get_area_covered(); %#ok
        goal_map.reset();
        m = m + 1;
    end
    
    dt = toc(tv);
    output.time_to_find_new_node(1,i) = dt;
    output.iterations(1,i) = i;
    try
        output.time_running(1,i) = output.time_running(1,i-1) + dt;
    catch
        output.time_running(1,i) = dt;
    end
    if output.time_running(1,i) > params.max_iteration_time
        final_nodes(m) = new_node;
        return
    end
    output.goal_reached = goal_reached;
    
    %Visualize
    if params.visualize ==true && mod(i,params.draw_freq) == 0
        visualize_growth(near_node.loc, new_node.loc, s_rand);
    end
    
end
final_nodes = final_nodes(1:(find(isnan([final_nodes.index]),1)-1));
nodes = nodes(1:(find(isnan([nodes.index]),1)-1));

disp('RRT-Path finished');
toc(tstart);
end