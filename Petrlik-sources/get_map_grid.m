function [ map_grid ] = get_map_grid( goal_index )
%GET_MAP_GRID Summary of this function goes here
%   Detailed explanation goes here

global params

cost_neighbor = 10;
cost_diagonal = 5;
% pravdìpodobnì cena, tato promìnná se nikde v kódu nejspíše dále nevyužije
% FUCK YOU AND YOUR NOT REVISED? NOT COMMENTED AND NOT REFACTORED CODE!!!
cst = zeros(params.world_dimensions(2,1)/params.cell_size, ...
    params.world_dimensions(2,1)/params.cell_size);

if params.algorithm==3
    % nechápu, proè, zapomenutá inicializace, algoritmus PSO pobìží o
    % trochu déle než by mìl se správnou inicializací
    for k=1:params.world_dimensions(2,1)/params.cell_size+1
        for m=1:params.world_dimensions(1,1)/params.cell_size+1
            map_grid(k,m) = contains_obj([(m-1)*params.cell_size;(k-1)*params.cell_size], ...
                [(m)*params.cell_size;(k)*params.cell_size], goal_index);
        end
    end
else
    map_grid = NaN(params.world_dimensions(2,1)/params.cell_size, ...
        params.world_dimensions(1,1)/params.cell_size);
    for k=1:params.world_dimensions(2,1)/params.cell_size
        for m=1:params.world_dimensions(1,1)/params.cell_size
            map_grid(k,m) = contains_obj([(m-1)*params.cell_size;(k-1)*params.cell_size], ...
                [(m)*params.cell_size;(k)*params.cell_size], goal_index);
        end
    end
    
    % myslím, že toto je umìlé zvìtšení pøekážek o 1 vrstvu, a try catche
    % jsou kvùli okrajùm mapy
    for k=1:params.world_dimensions(2,1)/params.cell_size
        for m=1:params.world_dimensions(1,1)/params.cell_size
            try
                if map_grid(k+1,m)==1
                    cst(k,m)=cst(k,m)+cost_neighbor;
                end
            catch
            end
            try
                if map_grid(k-1,m)==1
                    cst(k,m)=cst(k,m)+cost_neighbor;
                end
            catch
            end
            try
                if map_grid(k,m+1)==1
                    cst(k,m)=cst(k,m)+cost_neighbor;
                end
            catch
            end
            try
                if map_grid(k,m-1)==1
                    cst(k,m)=cst(k,m)+cost_neighbor;
                end
            catch
            end
            try
                if map_grid(k+1,m+1)==1
                    cst(k,m)=cst(k,m)+cost_diagonal;
                end
            catch
            end
            try
                if map_grid(k+1,m-1)==1
                    cst(k,m)=cst(k,m)+cost_diagonal;
                end
            catch
            end
            try
                if map_grid(k-1,m+1)==1
                    cst(k,m)=cst(k,m)+cost_diagonal;
                end
            catch
            end
            try
                if map_grid(k-1,m-1)==1
                    cst(k,m)=cst(k,m)+cost_diagonal;
                end
            catch
            end
        end
    end
end
end
