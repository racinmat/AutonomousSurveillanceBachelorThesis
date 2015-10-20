function [path] = get_path(nodes, last_node)

global empty_node

path = repmat(empty_node,1, last_node.index+1);
path(1) = last_node;
iter = 1;
while path(iter).prev ~= 0
    path(iter+1) = nodes(path(iter).prev);
    iter = iter + 1;
end

path = path(1:(find(isnan([path.index]),1)-1));

end