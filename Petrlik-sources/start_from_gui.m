function [  ] = start_from_gui(  )

global params goals nodes output

if params.random_init
    random_init_state();
end
tree_init();
if params.visualize || params.final_path
scene_init();
end

if length(goals)>1
    disp('More goals -> swarm splitting enabled automatically');
    params.swarm_splitting = true;
end
guiding_paths = get_guiding_path();
get_goal_distances(guiding_paths); % naplní nìjaké promìnné v output struktuøe délkami jednotlivých vedoucích cest (jedna cesta pro každou AoI)

switch params.algorithm;
    
    %RRT
    case 1
        final_nodes = rrt();
        [output.closest_node, output.distance_to_goal] = get_closest_node_to_goal();
        if ~any(output.goal_reached)
            last_node = output.closest_node;          
        else
            [last_node, ~] = get_best_fitness(final_nodes);
        end
        
        %RRT-Path
    case 2
        final_nodes = rrt_path(guiding_paths);
        [output.closest_node, output.distance_to_goal] = get_closest_node_to_goal();
        if ~any(output.goal_reached)
            last_node = output.closest_node;          
        else
            [last_node, ~] = get_best_fitness(final_nodes);
        end
        
        %PSO
    case 3
        [index, final_nodes, last_node] = pso([0 params.world_dimensions(1,1); ...
            0 params.world_dimensions(2,1)], params.initial_node);
        final_nodes = final_nodes(:,index(2))';
        nodes = final_nodes;
end

%Path visualization
path = get_path(last_node);

save_output();
if params.final_path
    visualize_path(path);
    
    visualize_best_config(last_node.loc);
    %waitforbuttonpress;
    visualize_steps(path);
end
end

