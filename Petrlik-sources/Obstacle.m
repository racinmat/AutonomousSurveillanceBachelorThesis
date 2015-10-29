classdef Obstacle
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        x
        y
        width
        height
    end
    
    methods
        
        function obs = Obstacle(x, y, width, height)
            global params
            
            obs.x = x-params.inflate_obstacles/2;
            obs.y = y-params.inflate_obstacles/2;
            obs.width = width+params.inflate_obstacles;
            obs.height = height+params.inflate_obstacles;
        end
        
        function draw(obs)
            global params
            
            infl = params.inflate_obstacles;
            rectangle('Position', [obs.x, obs.y, obs.width, obs.height], ...
                'EdgeColor', 'none', 'FaceColor', [100 38 38]./255);
            rectangle('Position', [obs.x+infl/2, obs.y+infl/2, ...
                obs.width-infl, obs.height-infl], ...
                'EdgeColor', 'none', 'FaceColor', [128 128 128]./255);
        end
    end
end

