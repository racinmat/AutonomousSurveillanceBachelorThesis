function [ final_nodes ] = rrt( )

global params number_of_uavs empty_node nodes goals goal_map stop output

disp('Starting RRT...');

nodes = tree_init();

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
 %       final_nodes(m) = new_node;
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
    if params.sampling_constraints
        s_rand = random_state_constraints(0, params.world_dimensions(1,1), ...
            0, params.world_dimensions(2,1), goal_reached);
    else
        s_rand = random_state_biased(goal_reached);
    end
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
    
    goal_reached = check_near_goal(new_node.loc);
    if all(~isnan(goal_reached))
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
disp('RRT finished');
toc(tstart);
end