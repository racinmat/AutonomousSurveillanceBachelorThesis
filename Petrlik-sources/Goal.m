classdef Goal
    %GOAL Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        x; y; width; height;
    end
    
    methods
        
        function g = Goal(varargin)
            g.x = varargin{2};
            g.y = varargin{3};
            g.width = varargin{4};
            g.height = varargin{5};
        end
        
        function draw(goal)
            rectangle('Position', [goal.x, goal.y, goal.width, goal.height], 'LineWidth', 1, 'FaceColor', [0.7 0.9 0.5], 'EdgeColor', [0.7 0.9 0.5]);
        end
        
        function near = is_near(goal, s_new)
            global params %#ok
            
            near = false;
            for m = 1:size(s_new, 2);
                if all(s_new(1,:) > goal.x) && all(s_new(1,:)<goal.x+goal.width) ...
                        && all(s_new(2,:)>goal.y) && all(s_new(2,:)<goal.y+goal.height)
                    near = true;
                end
            end
        end
    end
end
