function [ final_nodes, output ] = generate_rrt_fast( )

global number_of_uavs params empty_node nodes stop goals

disp('Starting RRT...');
tstart=tic;
nodes = tree_init();
final_nodes = repmat(empty_node, 1, params.max_nodes);
goal_reached = NaN(1,number_of_uavs);
values = [];

i = 1;
m = 1;
s = 2;
while (m <= params.number_of_solutions || i < params.min_nodes) && i < params.max_nodes
    if stop
        final_nodes(m) = new_node;
        return
    end
    if mod(i,100)==0
        fprintf('Nodes in tree: %d\n', i);
    end
    tv = tic;
    i = i + 1;
    if params.pso_optimization && all(~isnan(goal_reached))
        [final_node_index, particles] = pso([goals{1}.x goals{1}.x+goals{1}.width; ...
            goals{1}.y goals{1}.y+goals{1}.height], ...
            new_node);
        final_nodes = particles(final_node_index(1), final_node_index(2));
        nodes = nodes(1:(find(isnan([nodes.index]),1)-1));
        nodes = [nodes particles(:,final_node_index(2))']; %#ok
        return
    end
    
    if params.sampling_constraints
        s_rand = random_state_constraints(0, params.world_dimensions(1,1), ...
            0, params.world_dimensions(2,1));
    else
        s_rand = random_state_biased();
    end
    k = 0;
    near_found = false;
    
    %Finding appropriate nearest neighbor
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
    nodes(i) = new_node;
    s = s + 1;
    
    %Visualize
    if params.visualize ==true && mod(i,params.draw_freq) == 0
        visualize_growth(near_node.loc, new_node.loc, s_rand);
    end
    
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
end
final_nodes = final_nodes(1:(find(isnan([final_nodes.index]),1)-1));
nodes = nodes(1:(find(isnan([nodes.index]),1)-1));

disp('RRT finished');
toc(tstart)
end



