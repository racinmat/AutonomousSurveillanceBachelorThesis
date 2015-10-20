function [  ] = start_from_gui(  )

global params goals nodes output

tree_init();

switch params.algorithm;
    
    %RRT
    case 1
        final_nodes = generate_rrt_fast();
        [last_node, ~] = get_best_fitness(final_nodes);
        
        %RRT-Path
    case 2
        for m=1:length(goals)
            map_grid = get_map_grid(m);
            grid_nodes = grid_to_nodes( map_grid );           
            [i, j] =  find(map_grid == 2, 1);
            p1 = i*size(map_grid,2)+j;
            [i, j] =  find(map_grid == 3, 1);
            p2 = i*size(map_grid,2)+j;
            guiding_path = a_star(p1, p2, grid_nodes);
            guiding_paths{m} = guiding_path; %#ok
            plot([guiding_path.x], [guiding_path.y], 'color', [170 222 135]./255, 'linewidth', 2)
            hold on
            drawnow
        end
        final_nodes = rrt_path(guiding_paths);
        [last_node, ~] = get_best_fitness(final_nodes);
        
        %PSO
    case 3
        [index, final_nodes, last_node] = pso([0 params.world_dimensions(1,1); ...
            0 params.world_dimensions(2,1)], params.initial_node);
        final_nodes = final_nodes(:,index(2))';
        nodes = final_nodes;
end

%Path visualization
path = get_path(nodes, last_node);
output.path = path;
visualize_path(path);

visualize_best_config(last_node.loc);
waitforbuttonpress;
visualize_steps(path);
end

