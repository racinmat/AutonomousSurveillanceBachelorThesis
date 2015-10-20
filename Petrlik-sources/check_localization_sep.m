function [ in_range, neighbors ] = check_localization_sep( node )
%CHECK_LOCALIZATION Checks relative localization of UAVs
%   Detailed explanation goes here

global number_of_uavs params

%Initialize default values
neighbors = zeros(1,number_of_uavs);

%Single UAV does not need localization
if number_of_uavs == 1;
    in_range = true;
    return
end

%Check minimal distance between UAVs
for m = 1:number_of_uavs-1
    for n = m+1:number_of_uavs
        if sqrt((node.loc(1,m)-node.loc(1,n))^2 + (node.loc(2,m)-node.loc(2,n))^2) < params.relative_distance.min
            in_range = false;
            return
        end
    end
end

%Check maximal distance between UAVs
for m = 1:number_of_uavs-1
    for n = m+1:number_of_uavs
        if sqrt((node.loc(1,m)-node.loc(1,n))^2 +  (node.loc(2,m)-node.loc(2,n))^2) ...
                < params.relative_distance.max ...
                && (~params.check_fov ...
                || (abs(node.rot(3,m)-node.rot(3,n)) < params.localization.angle/2))
            neighbors(1,m) = neighbors(1,m) + 1;
            neighbors(1,n) = neighbors(1,n) + 1;
            if all(neighbors>=params.required_neighbors)
                in_range = true;
                return
            end
        end
    end
end

%Check whether each UAV has required number of neighbors
if all(neighbors>=params.required_neighbors)
    in_range = true;
    return
else
    in_range = false;
    return
end
end