classdef Goal_map < handle
    %GOALMAP Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        p_size
        m_size
        element_size
        goals
        matrix
        init_matrix
        gm
    end
    
    methods
        
        function goal_map = Goal_map(x_size, y_size, element_size)
            goal_map.p_size.x = x_size;
            goal_map.p_size.y = y_size;
            goal_map.element_size = element_size;
            goal_map.m_size.x = round(x_size/element_size);
            goal_map.m_size.y = round(y_size/element_size);
            goal_map.matrix = zeros(round(y_size/element_size),round(x_size/element_size));
        end
        
        function add_goal(map, varargin)
            rect.x = round(varargin{1}/map.element_size);
            rect.y = round(varargin{2}/map.element_size);
            rect.width = round(varargin{3}/map.element_size);
            rect.height = round(varargin{4}/map.element_size);
            rect.value = varargin{5};
            rect.matrix = zeros(rect.width,rect.height);
            rect.matrix(:,:) = rect.value;
            map.goals{end+1} = rect;
            map.matrix(rect.y:rect.y+rect.height, rect.x:rect.x+rect.width) = rect.value;
            map.init_matrix = map.matrix;
        end
        
        function draw(map)
            global figures
            
            set(0, 'CurrentFigure', figures.fig4);
            colormap(jet);
            image(flipud(map.matrix));
            drawnow
            hold off
        end
        
        function apply_camera(map, x, y, width, height, world_dimensions)
            x = round(x/map.element_size);
            if x < 1
                x = 1;
            end
            
            y = round(y/map.element_size);
            if y < 1
                y  = 1;
            end
            
            width = round(width/2/map.element_size);
            height = round(height/2/map.element_size);
            
            if y+height>world_dimensions(2,1) && x+width>world_dimensions(1,1)
                map.matrix(y:world_dimensions(2,1), x:world_dimensions(1,1)) = floor(map.matrix(y:world_dimensions(2,1), x:world_dimensions(1,1))/2);
            elseif y+height>world_dimensions(2,1)
                map.matrix(y:world_dimensions(2,1), x:x+width) = floor(map.matrix(y:world_dimensions(2,1), x:x+width)/2);
            elseif x+width>world_dimensions(1,1)
                map.matrix(y:y+height, x:world_dimensions(1,1)) = floor(map.matrix(y:y+height, x:world_dimensions(1,1))/2);
            elseif ~isnan(x) && ~isnan(y)
                map.matrix(y:y+height, x:x+width) = floor(map.matrix(y:y+height, x:x+width)/2);
            end
            
            if y+height>world_dimensions(2,1) && x-width<1
                map.matrix(y:world_dimensions(2,1), 1:x-1) = round(map.matrix(y:world_dimensions(2,1), 1:x-1)/2);
            elseif y+height>world_dimensions(2,1)
                map.matrix(y:world_dimensions(2,1), x-width:x-1) = round(map.matrix(y:world_dimensions(2,1), x-width:x-1)/2);
            elseif x-width<1
                map.matrix(y:y+height, 1:x-1) = round(map.matrix(y:y+height, 1:x-1)/2);
            elseif ~isnan(x) && ~isnan(y)
                map.matrix(y:y+height, x-width:x-1) = round(map.matrix(y:y+height, x-width:x-1)/2);
            end
            
            if y-height<1 && x-width<1
                map.matrix(1:y-1, 1:x-1) = round(map.matrix(1:y-1, 1:x-1)/2);
            elseif x-width<1
                map.matrix(y-height:y-1, 1:x-1) = round(map.matrix(y-height:y-1, 1:x-1)/2);
            elseif y-height<1
                map.matrix(1:y-1, x-width:x-1) = round(map.matrix(1:y-1, x-width:x-1)/2);
            elseif ~isnan(x) && ~isnan(y)
                map.matrix(y-height:y-1, x-width:x-1) = round(map.matrix(y-height:y-1, x-width:x-1)/2);
            end
            
            if y-height<1 && x+width>world_dimensions(1,1)
                map.matrix(1:y-1, x:world_dimensions(1,1)) = round(map.matrix(1:y-1, x:world_dimensions(1,1))/2);
            elseif y-height<1
                map.matrix(1:y-1, x:x+width) = round(map.matrix(1:y-1, x:x+width)/2);
            elseif x+width>world_dimensions(1,1)
                map.matrix(y-height:y-1, x:world_dimensions(1,1)) = round(map.matrix(y-height:y-1, x:world_dimensions(1,1))/2);
            elseif ~isnan(x) && ~isnan(y)
                map.matrix(y-height:y-1, x:x+width) = round(map.matrix(y-height:y-1, x:x+width)/2);
            end
        end
        
        function total = get_sum(map)
            total = sum(sum(map.matrix));
        end
        
        function part_covered = get_area_covered(map)
            goal_area = length(map.matrix(map.matrix>0));
            covered_area = goal_area - length(map.matrix(map.matrix==100));
            part_covered = covered_area/goal_area;
        end
        
        function reset(map)
            map.matrix = map.init_matrix;
        end
    end
end

