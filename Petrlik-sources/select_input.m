function [near_node, new_node ] = select_input( s_rand, near_node )
%SELECT_INPUT Selects vector which takes the UAV closest to s_rand

global number_of_uavs params empty_node

inputs = NaN(params.input_samples_phi*params.input_samples_dist,2);
d = zeros((params.input_samples_phi*params.input_samples_dist)^number_of_uavs,1);
new_node = empty_node;
translation = NaN(2,number_of_uavs,(params.input_samples_phi*params.input_samples_dist)^number_of_uavs);
%index = NaN(1,number_of_uavs);

for k = 0:params.input_samples_dist-1
    for m = 0:params.input_samples_phi-1
        inputs(k*params.input_samples_phi+m+1,:) = ...
            [params.distance_of_new_nodes/1.5^k ...
            -params.max_turn+2*m*params.max_turn/(params.input_samples_phi-1)];
    end
end

if params.zero_step
    inputs(1:end,1:end,end+1) = 0;
end

for n=1:number_of_uavs
    first_input(:,n,1) = inputs(1,:);
end
current_input = first_input;
combinations(:,:,1) = current_input;

n = 1;
m = 1;
last = number_of_uavs;
input_index(1,1:number_of_uavs) = 1;
index = 0;
while true
    
    
    %     current_input(1,:,m) = inputs(input_index(1,m),:);
    while input_index(1,m) > length(inputs)
        %index = index + 1;
        %          combinations(index,:,:) = current_input;
        input_index(1,m+1) = input_index(1,m+1) + 1;
        current_input(:,m,1) = first_input(:,m,1);
        
        input_index(1,m) = 1;
        m = m + 1;
        if m > number_of_uavs
            break
        end
        
    end
    current_input(:,m) = inputs(input_index(1,m),:);
    index = index + 1;
    combinations(:,:,index) = current_input;
    
    m = 1;
    input_index(1,m) = input_index(1,m) + 1;
    if all(input_index(1,:) >= length(inputs)) ...
            && input_index(1,1) > length(inputs)
        break
    end
    
end

clear inputs
inputs = combinations;

for m = 1:size(inputs,3);
    
    %Calling car like motion model
    [tmp_node, ~] = car_like_motion_model(near_node, inputs(:,:,m));
    tmp_nodes(m) = tmp_node; %#ok
    for n = 1:number_of_uavs
        translation(1:2,n,m) = [tmp_node.loc(1,n) - near_node.loc(1,n); ...
            tmp_node.loc(2,n) - near_node.loc(2,n)];
    end
end

%near_node = check_obstacle_vcollide(near_node, translation);

%Distance to s_rand when using different inputs
for m = 1:size(inputs,3);
    tmp_node = tmp_nodes(m);
    for n=1:number_of_uavs
        d(m) = d(m) + norm([s_rand(1,n) - tmp_node.loc(1,n) ...
            s_rand(2,n) - tmp_node.loc(2,n)]);
    end
end

tmp_node = empty_node;

%Find vector with minimal distance to s_rand and return it
if  params.relative_localization == true
    m = 0;
    while m < size(inputs,3)
        m = m + 1;
        if all(near_node.used_inputs)
            %disp('No valid input left');
            break
        end
        index = find(d(:) == min(d(:)),1);
        tmp_node = tmp_nodes(index);
        if length(tmp_node)<1
            break
        end
        
        if ~check_localization_sep(tmp_node) ...
                || trajectory_intersection(near_node, tmp_node) ...
                || near_node.used_inputs(index,1)
            d(index) = NaN;
            continue
        end
        
        if ~check_world_bounds(tmp_nodes(index).loc)
            d(index) = NaN;
            continue
        end
        
        near_node = check_obstacle_vcollide_single(near_node, translation, index);
        
        if near_node.used_inputs(index,1)
            d(index) = NaN;
            continue
        else
            new_node.loc = tmp_nodes(index).loc;
            new_node.rot = tmp_nodes(index).rot;
            new_node.prev = near_node.index;
            new_node.prev_inputs(:,:) = tmp_nodes(index).prev_inputs(:,:);
            near_node.used_inputs(index,1) = true;
            break
        end
    end
else
    if ~near_node.used_inputs(m,1)
        index = find(d(:) == min(d(:)),1);
        tmp_node.loc = tmp_nodes(index(1,n)).loc;
        tmp_node.rot = tmp_nodes(index(1,n)).rot;
        new_node = tmp_node;
        new_node.prev = near_node.index;
        new_node.prev_inputs(:,:) = tmp_nodes(index(1,n).prev_inputs(:,:));
        near_node.used_inputs(index,1) = true;
    end
end


