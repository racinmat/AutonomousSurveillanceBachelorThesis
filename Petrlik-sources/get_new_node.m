function [new_node] = get_new_node(near_node, u, step)

global number_of_uavs empty_node

c = NaN(1,number_of_uavs);
new_node = empty_node;

for n = 1:number_of_uavs
    c(1,n) = step(1,n) / norm(u(1:2,n));
    new_node.loc(1:2,n) = near_node.loc(1:2,n) + c(1,n)*u(1:2,n);
    new_node.loc(3,n) = 0;
    new_node.rot(3,n) = atan2(new_node.loc(2,n) - near_node.loc(2,n), ...
        new_node.loc(1,n) - near_node.loc(1,n));
end
new_node.prev = near_node.index;
end
