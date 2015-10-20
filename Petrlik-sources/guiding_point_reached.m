function [ current_index ] = guiding_point_reached( node, guiding_paths, current_index )
%GUIDING_POINT_REACHED Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs params

for k=1:length(guiding_paths)
    guiding_path = guiding_paths{k};
    for m=1:length(guiding_path)
        for n=1:number_of_uavs
            reached = false;

            
            if ((node.loc(1,n)-guiding_path(m).x)^2 + (node.loc(2,n)-guiding_path(m).y)^2 ...
                    < params.guiding_near_dist^2)
                reached = true;
            %Narrow passage detection
            detect_narrow_passage(guiding_path(m));
            end
            if reached==true && m>1 && m<=current_index(n,k)
                current_index(n,k) = m-1;
                
                break
            end
        end
    end
end
end