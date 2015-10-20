function [ unexpandable_count ] = check_expandability( )
%CHECK_EXPANDABILITY Summary of this function goes here
%   Detailed explanation goes here

    global nodes
    
    unexpandable_count = 0;
    all_nodes = nodes(~isnan([nodes.index]));
    
    for k=1:length(all_nodes)
        if any(all(all_nodes(k).used_inputs==1))
            unexpandable_count = unexpandable_count + 1;
        end
    end
    
    if unexpandable_count>0
      %  fprintf('Not expandable nodes: %d/%d\n', unexpandable_count, length(all_nodes));
    end
    
end

