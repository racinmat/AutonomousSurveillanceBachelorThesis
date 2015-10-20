function [ path ] = a_star( start, goal, nodes )

disp('A* guiding path planning starting...');
tic
empty_node = struct('id', NaN, 'x', NaN, 'y', NaN, 'neighbors', NaN(1,8), ...
    'g_score', Inf, 'f_score', Inf, 'came_from', NaN);
closedset = empty_node;
openset(1) = nodes(start);

openset(1).g_score = 0;
openset(1).f_score = openset(1).g_score + heuristic_cost_estimate(openset(1), nodes(goal));
openset(1).came_from = 0;

%While openset is not empty
while ~is_empty(openset)
    
    current = lowest_f_score(openset);
    
    %End algorithm when goal reached
    if current.id == nodes(goal).id
        path = reconstruct_path(current, closedset);
        disp('A* finished.')
        toc
        return
    end
    
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
            neighbor.f_score = neighbor.g_score + heuristic_cost_estimate(neighbor, nodes(goal));
            neighbor.came_from = current.id;
            openset(end+1) = neighbor; %#ok
        elseif tentative_g_score < openset([openset.id] == neighbor.id).g_score
            neighbor.g_score = tentative_g_score;
            neighbor.f_score = neighbor.g_score + heuristic_cost_estimate(neighbor, nodes(goal));
            neighbor.came_from = current.id;
            openset([openset.id] == neighbor.id) = neighbor;
        end
    end
end

disp('A* failed');
end

%Returns path found by A*
function path = reconstruct_path(current, closedset)
path(1) = current;
while current.came_from ~= 0
    current = closedset([closedset.id] == current.came_from);
    path(end+1) = current; %#ok
end
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

%Returns heuristic cost estimate of neighbor node and goal
function dist = heuristic_cost_estimate(neighbor, goal)
dist = sqrt((neighbor.x-goal.x)^2+(neighbor.y-goal.y)^2);
end


