function [path] = get_path( last_node)

global empty_node number_of_uavs nodes output

path = repmat(empty_node,1, last_node.index+1);
path(1) = last_node;
iter = 1;
while path(iter).prev ~= 0
    path(iter+1) = nodes(path(iter).prev);
    iter = iter + 1;
end

path = path(1:(find(isnan([path.index]),1)-1));

geo_path_length = 0;
for m=2:length(path)
    for n=1:number_of_uavs
    geo_path_length = geo_path_length + ...
        sqrt((path(m).loc(1,n)-path(m-1).loc(1,n))^2 + ...
        (path(m).loc(2,n)-path(m-1).loc(2,n))^2);
    end
end
geo_path_length = geo_path_length/number_of_uavs;

output.geometric_path_length = geo_path_length;
output.path = path;
output.runtime = nodes(end).time_added;
output.curvature = get_curvature(path);

end