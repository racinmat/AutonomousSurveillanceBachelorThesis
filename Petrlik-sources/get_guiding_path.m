function [ guiding_paths ] = get_guiding_path(  )
%GET_GUIDING_PATH Summary of this function goes here
%   Detailed explanation goes here

global params goals

for m=1:length(goals)
    map_grid = get_map_grid(m); % celý ètverec se spoèítá do jeho levého horního (nejsem si jist, provìøit) bodu, tím pádem se mapa "posune"
    % toto "posunutí" zpùsobuje problémy, pokud je pøekážka o 1
    % blok "zvìtšena", tím se zvìtší pouze vlevo, ale nikoli vpravo
            
    % promìnné v matici map_grid: 
    % 3 je buòka s cílem
    % 2 je buòka s UAV
    % 1 je buòka s pøekážkou
    % 0 ke prázdná buòka

    if params.grid_to_nodes_old
        grid_nodes = grid_to_nodes_old( map_grid );
    else
        grid_nodes = grid_to_nodes( map_grid );
    end
    [i, j] =  find(map_grid == 2, 1); % node s UAV nejvíce vlevo dole (2. parametr u metody find je 1)
    p1 = i*size(map_grid,2)+j;  % chyba s posunutím o 1 øádek výše, matlab indexuje od 1 a tak by mìl od i odeèíst 1, aby bylo id správnì
    [i, j] =  find(map_grid == 3, 1); % node s cílem nejvíce vlevo dole (2. parametr u metody find je 1)
    p2 = i*size(map_grid,2)+j;  % chyba s posunutím o 1 øádek výše, matlab indexuje od 1 a tak by mìl od i odeèíst 1, aby bylo id správnì
    guiding_path = a_star(p1, p2, grid_nodes);

    % pøidal jsem
    assignin('base', 'p1', p1);
    assignin('base', 'p2', p2);
    assignin('base', 'grid_nodes', grid_nodes);
    assignin('base', 'guiding_path', guiding_path);
    assignin('base', 'map_grid', map_grid);
    % konec pøidání
    
    guiding_paths{m} = guiding_path; %#ok
    if params.algorithm==2 && params.final_path
        plot([guiding_path.x], [guiding_path.y], 'color', [170 222 135]./255, 'linewidth', 2)
        hold on
        drawnow
    end
end

end

