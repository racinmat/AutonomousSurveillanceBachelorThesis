function [ best_node, map, value] = get_best_fitness( final_nodes )
%FITNESS_FUNCTION Summary of this function goes here
%   Detailed explanation goes here

fit_tmp = NaN(1,length(final_nodes));

for m = 1:length(final_nodes)+1
    if m > length(final_nodes) || isnan(final_nodes(m).index)
        m = m - 1; %#ok
        fit = fit_tmp(1,1:m);
        [~, index] = min(fit(1,:));
        best_node = final_nodes(index);
        [value, map] = fitness_function(best_node.loc);
        map.reset();
        break
    end
    [fit_tmp(m), map] = fitness_function(final_nodes(1,m).loc);
    map.reset();
end
end