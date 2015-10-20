function [ closedset ] = distance_transform( start, nodes )
%A_STAR Summary of this function goes here
%   Detailed explanation goes here
disp('Distance transform starting...');
tic
empty_node = struct('id', NaN, 'x', NaN, 'y', NaN, 'neighbors', NaN(1,8), ...
    'cost', 1, 'g_score', Inf, 'f_score', Inf, 'came_from', NaN);
closedset = empty_node;
openset(1) = nodes(start);

openset(1).g_score = 0;
openset(1).f_score = openset(1).g_score;
openset(1).came_from = 0;

%While openset is not empty
while ~is_empty(openset)
    current = lowest_f_score(openset);
    
    %Remove current node from openset
    openset([openset.id] == [current.id]).id = 0; %#ok
    %Add current node to closedset
    closedset(end+1) = current; %#ok
    
    for m = 1:length(current.neighbors)
        if isnan(current.neighbors(m))
            break
        end
        neighbor = nodes(current.neighbors(m));
        neighbor.g_score = Inf;
        if contains(closedset, neighbor)
            continue
        end
        
        tentative_g_score = current.g_score ...
            + dist_between(current, neighbor);
        if ~contains(openset, neighbor)
            neighbor.g_score = tentative_g_score;
            neighbor.f_score = neighbor.g_score;
            neighbor.came_from = current.id;
            openset(end+1) = neighbor; %#ok
        elseif tentative_g_score < openset([openset.id] == neighbor.id).g_score
            neighbor.g_score = tentative_g_score;
            neighbor.f_score = neighbor.g_score;
            neighbor.came_from = current.id;
            openset([openset.id] == neighbor.id) = neighbor;
        end
    end
end

disp('Distance transform finished');
end

%Checks whether the set contains input node
function bool = contains(set, node)
if any(ismember([set.id],node.id))
    bool = true;
else
    bool = false;
end

end

%Checks whether the set is empty
function bool = is_empty(set)
bool = true;
for k = 1:length(set)
    if ~isnan(set(k).id) && set(k).id > 0
        bool = false;
        return
    end
end
end

%Returns the node with lowest f_score
function node = lowest_f_score(openset)
node = openset([openset.id] > 0);
node = node(1);
for k = 1:length(openset)
    if (openset(k).id > 0) && (openset(k).f_score < node.f_score)
        node = openset(k);
    end
end
end

%Returns distance of two nodes
function dist = dist_between(node1, node2)
dist = sqrt((node1.x-node2.x)^2+(node1.y-node2.y)^2);
end



