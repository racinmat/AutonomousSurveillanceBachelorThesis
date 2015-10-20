function [ guiding_point ] = random_guiding_path( guiding_paths, index_reached )
%RANDOM_GUIDING_POINT Summary of this function goes here
%   Detailed explanation goes here

sum = 0;
for m=1:length(guiding_paths)
    l = length(guiding_paths(m));
    probabilities(1,m) = l; %#ok
    sum = sum + l;
end

probabilities = probabilities./sum;
rn = rand();
add = 0;
for m=1:length(probabilities)
    if rn<probabilities(m)+add
        guiding_path = guiding_paths{m};
        break
    else
        add = add + probabilities(m);
    end
end

guiding_point = [guiding_path(index_reached-1).x; ...
    guiding_path(index_reached-1).y];
end

