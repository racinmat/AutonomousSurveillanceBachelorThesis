function [ nodes ] = grid_to_nodes( grid )
%GRID_TO_NODES Summary of this function goes here
%   Detailed explanation goes here
global params

y_count = size(grid, 1);
x_count = size(grid, 2);
node = struct('id', NaN, 'x', NaN, 'y', NaN, 'neighbors', NaN(1,8), 'cost', 1);
nodes = repmat(node, 1, y_count*x_count);
id = 0;
cost_neighbor = 10;
cost_diagonal = 5;

for k=1:y_count
    for m=1:x_count
        id = id + 1;  
        if grid(k,m)~=1                    
            nodes(id).id = id;
            nodes(id).x = (m-1)*params.cell_size+params.cell_size/2;
            nodes(id).y = (k-1)*params.cell_size+params.cell_size/2;
            n_index = 0;
            for p = -1:1
                for q = -1:1
                    if (p~=0 || q~=0) ...
                            && k+p>=1 && m+q>=1 && k+p<=y_count && m+q<=x_count ...
                            && (grid(k+p,m+q)~=1 )
                        n_index = n_index+1;
                        nodes(id).neighbors(n_index) = (k+p-1)*x_count+(m+q);
                    end
                end
            end
                        
            try
            if grid(k+1,m)==1
                nodes(id).cost=nodes(id).cost+cost_neighbor;
            end
            catch
            end
            try
            if grid(k-1,m)==1
                nodes(id).cost=nodes(id).cost+cost_neighbor;
            end
            catch
            end
            try
            if grid(k,m+1)==1
                nodes(id).cost=nodes(id).cost+cost_neighbor;
            end
            catch
            end
            try
            if grid(k,m-1)==1
                nodes(id).cost=nodes(id).cost+cost_neighbor;
            end
            catch
            end
            try
            if grid(k+1,m+1)==1
                nodes(id).cost=nodes(id).cost+cost_diagonal;
            end
            catch
            end
            try
            if grid(k+1,m-1)==1
                nodes(id).cost=nodes(id).cost+cost_diagonal;
            end
            catch
            end
            try
            if grid(k-1,m+1)==1
                nodes(id).cost=nodes(id).cost+cost_diagonal;
            end
            catch
            end
            try
            if grid(k-1,m-1)==1
                nodes(id).cost=nodes(id).cost+cost_diagonal;
            end
            catch
            end
            
           
        end
    end
    
% nodes = nodes(1:find([nodes.id],1,'last'));
end

