function [near_node] = nearest_neighbor(s_rand, nodes, count)

global number_of_uavs params empty_node

near_arr = repmat(empty_node, 1, 1);
near_node = nodes(1);
s = 1;
current_best = Inf;

for j = 1:params.max_nodes
    %Distance of next node in the tree
    tmp_node = nodes(j);
    if any(any(isnan(tmp_node.loc)))
        if params.debug
            fprintf('NaN in node %d\n',tmp_node.index);
        end
        break
    end
    if all(tmp_node.used_inputs)
        %fprintf('Node %d is unexpandable\n',tmp_node.index);
        continue
    end
    
    hamilt_dist = 0;
    if params.nn_method == 1
        for n = 1:number_of_uavs
            hamilt_dist = hamilt_dist + (tmp_node.loc(1,n) - s_rand(1,n))^2 ...
                + (tmp_node.loc(2,n) - s_rand(2,n))^2;
        end
    elseif params.nn_method == 2
        for n = 1:number_of_uavs
            dst(1,n) = (tmp_node.loc(1,n) - s_rand(1,n))^2 ...
                + (tmp_node.loc(2,n) - s_rand(2,n))^2;
        end
        hamilt_dist = max(dst);
    elseif params.nn_method == 3
        for n = 1:number_of_uavs
            dst(1,n) = (tmp_node.loc(1,n) - s_rand(1,n))^2 ...
                + (tmp_node.loc(2,n) - s_rand(2,n))^2;
        end
        hamilt_dist = min(dst);
    end
    
    
    %Check if tested node is nearer than the current nearest
    if hamilt_dist < current_best
        near_arr(s) = near_node;
        current_best = hamilt_dist;
        near_node = nodes(j);
        if params.debug
            for n=1:number_of_uavs
                dist = sqrt((near_node.loc(1,n) - s_rand(1,n))^2 ...
                    + (near_node.loc(2,n) - s_rand(2,n))^2);
            end
            fprintf('[debug] near node #%d found, distance to goal state: %d\n', s, dist);
        end
        s = s + 1;
    end
    
end

if length(near_arr) > count
    near_node = near_arr(end-count);
    %   if params.debug
    for n=1:number_of_uavs
        dist = sqrt((near_node.loc(1,n) - s_rand(1,n))^2 ...
            + (near_node.loc(2,n) - s_rand(2,n))^2);
    end
    if params.debug && count > 0
        fprintf('[debug] near node #%d chosen, %d discarded, near node index %d, distance to goal state: %d\n',length(near_arr)-count, count, near_node.index, dist);
    end
    %  end
end
end