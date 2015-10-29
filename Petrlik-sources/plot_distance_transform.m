clear all
close all

global number_of_uavs params goals maps obstacles

prepare_parameters();
prepare_maps();
params.map = 1;
params.world_dimensions = [1000;1000];
number_of_uavs = 1;
params.cell_size = 50;

params.initial_node = maps(1,params.map).init_node;
obstacles = maps(1,params.map).obstacles;
goals = maps(1,params.map).goals;

for m=1:length(goals)
    map_grid = get_map_grid(m);
    map_grid = flipud(map_grid);
    grid_nodes = grid_to_nodes( map_grid );
    [i, j] =  find(map_grid == 3, 1);
    p2 = i*size(map_grid,2)+j;
    dist = distance_transform(p2, grid_nodes);
    distances{m} = dist;
end

x = round([distances{1,1}.x]);
x = x(:)+1;
y = round([distances{1,1}.y]);
y = y(:)+1;
distance_matrix = zeros(round(max(y)/params.cell_size),round(max(x)/params.cell_size));
for n=1:length(goals)
x = round([distances{1,n}.x]);
x = x(:)+1;
y = round([distances{1,n}.y]);
y = y(:)+1;
c = [distances{1,n}.g_score];


for m=1:length(c)
    if isfinite(x(m)) && isfinite(y(m))
        % fprintf('%d %d\n', x(m), y(m));
        distance_matrix(round(y(m)/50),round(x(m)/50))= ...
            distance_matrix(round(y(m)/50),round(x(m)/50)) + c(m);
        
    end
end
end

image(distance_matrix, 'CDataMapping','scaled');
colorbar