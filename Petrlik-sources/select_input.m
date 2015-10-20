function [near_node, new_node ] = select_input( s_rand, near_node )
%SELECT_INPUT Selects vector which takes the UAV closest to s_rand

global number_of_uavs params empty_node

inputs = NaN(2,number_of_uavs,params.input_samples_phi*params.input_samples_dist);
d = NaN(1,number_of_uavs,size(inputs,3));
new_node = near_node;
translation = NaN(2,number_of_uavs,size(inputs,3));
index = NaN(1,number_of_uavs);

for k = 0:params.input_samples_dist-1
    for m = 0:params.input_samples_phi-1
        inputs(1:end,1:end,k*params.input_samples_phi+m+1) = ...
            [repmat(params.distance_of_new_nodes/1.5^k,1,number_of_uavs); ...
            repmat(-params.max_turn+2*m*params.max_turn/(params.input_samples_phi-1),1,number_of_uavs)];
    end
end

if params.zero_step
    inputs(1:end,1:end,end+1) = 0;
end

for m = 1:size(inputs,3);
    
    %Calling car like motion model
    [tmp_node, ~] = car_like_motion_model(near_node, inputs(:,:,m));
    tmp_nodes(m) = tmp_node; %#ok
    for n = 1:number_of_uavs
        translation(1:2,n,m) = [tmp_node.loc(1,n) - near_node.loc(1,n); ...
            tmp_node.loc(2,n) - near_node.loc(2,n)];
    end
end

near_node = check_obstacle_vcollide(near_node, translation);

%Distance to s_rand when using different inputs
for m = 1:size(inputs,3);
    for n = 1:number_of_uavs
        tmp_node = tmp_nodes(m);
        if ~near_node.used_inputs(m,n) && check_world_bounds(tmp_node.loc(1:2,n))
            d(1,n,m) = norm([s_rand(1,n) - tmp_node.loc(1,n) ...
                s_rand(2,n) - tmp_node.loc(2,n)]);
        else
            near_node.used_inputs(m,n) = true;
            d(1,n,m) = NaN;
        end
    end
end

tmp_node = empty_node;

%Find vector with minimal distance to s_rand and return it
for n = 1:number_of_uavs
    if  params.relative_localization == true
        m = 0;
        while m < size(inputs,3)
            m = m + 1;
            if ~near_node.used_inputs(m,n) && ~all(isnan(d(1,n,:)))
                index(1,n) = find(d(1,n,:) == min(d(1,n,:)),1);
                tmp_node = tmp_nodes(index(1,n));
                if ~check_localization_sep(tmp_node)
                    d(1,n,index(1,n)) = NaN;
                    continue
                else
                    new_node.loc(:,n) = tmp_nodes(index(1,n)).loc(:,n);
                    new_node.rot(:,n) = tmp_nodes(index(1,n)).rot(:,n);
                    new_node.prev = near_node.index;
                    new_node.prev_inputs(:,n) = tmp_nodes(index(1,n)).prev_inputs(:,n);
                    near_node.used_inputs(m,n) = true;
                    break
                end
            end
        end
    else
        if ~near_node.used_inputs(m,n)
            index(1,n) = find(d(1,n,:) == min(d(1,n,:)),1);
            tmp_node.loc(:,n) = tmp_nodes(index(1,n)).loc(:,n);
            tmp_node.rot(:,n) = tmp_nodes(index(1,n)).rot(:,n);
            new_node = tmp_node;
            new_node.prev = near_node.index;
            new_node.prev_inputs(:,n) = tmp_nodes(index(1,n).prev_inputs(:,n));
            near_node.used_inputs(index(1,n),n) = true;
        end
    end
end

