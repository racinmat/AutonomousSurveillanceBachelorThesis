function [ s_rand ] = random_state_guided( guiding_paths, index, goal_reached )
%RANDOM_STATE_GUIDED Summary of this function goes here
%   Detailed explanation goes here
% v promìnné index je matice, sloupec pro cestu, øádek pro UAV. øíká, kolik
% nodes zbývá UAV než dorazí na konec cesty

global number_of_uavs params

sum = 0; % sum = celková cesta všech vedoucích cest
for m=1:length(guiding_paths)
    l = length(guiding_paths{m}); % èím delší cesta, tím vìtší pravdìpodobnost, že se tou cestou vydá
    probabilities(1,m) = l; %#ok propabilities je normální, 1D pole. nechápu, proè tady používá notaci pro 2D pole
    sum = sum + l;
end

probabilities = probabilities./sum;
s_rand = NaN(2,number_of_uavs);

%Random path sampled for each uav
if params.rrt_path_sampling_case == 1
    for n=1:number_of_uavs
        rn2 = rand();   % urèuje, zda se vybere náhodný bod nebo bod na vedoucí cestì
        if rn2 > params.guided_sampling_prob
            s_rand(:,n) = random_state(0, params.world_dimensions(1,1), ...
                0, params.world_dimensions(2,1));
        else                % bod na vedoucí cestì
            rn = rand();
            add = 0;
            for m = 1:length(probabilities) % length(propabilities) je poèet vedoucích cest
                if rn < probabilities(m) + add
                    guiding_path = guiding_paths{m};
                    center = [guiding_path(index(n,m)).x; ...
                        guiding_path(index(n,m)).y];
                    if goal_reached(1,n) == m
                        s_rand(:,n) = random_state_goal(goal_reached(1,n));
                    else
                        while true
                            s_rand(:,n) = random_state_polar(center, 0, params.sampling_radius);
%                            if ~check_inside_obstacle(s_rand(:,n))
                                break
%                            end
                        end
                        break
                    end
                else
                    add = add + probabilities(m);
                end
            end
        end
    end
    
    %Same path sampled for each uav
elseif params.rrt_path_sampling_case == 2
    rn2 = rand();
    if rn2 > params.guided_sampling_prob
        for n=1:number_of_uavs
            s_rand(:,n) = random_state(0, params.world_dimensions(1,1), ...
                0, params.world_dimensions(2,1));
        end
    else
        rn = rand();
        add = 0;
        for m=1:length(probabilities)
            
            if rn < probabilities(m) + add
                for n=1:number_of_uavs
                    guiding_path = guiding_paths{m};
                    center = [guiding_path(index(n,m)).x; ...
                        guiding_path(index(n,m)).y];
                    if goal_reached(1,n) == m
                        s_rand(:,n) = random_state_goal(goal_reached(1,n));
                    else
                        while true
                            s_rand(:,n) = random_state_polar(center, 0, params.sampling_radius);
                            if ~check_inside_obstacle(s_rand(:,n))
                                break
                            end
                        end
                        break
                    end
                end
                break
            else
                add = add + probabilities(m);
            end
        end
    end
    
    %Every UAV has its guiding path
elseif params.rrt_path_sampling_case == 3
    rn2 = rand();
    if rn2 > params.guided_sampling_prob    % vybírá se náhodný vzorek
        for n=1:number_of_uavs
            if goal_reached(1,n) == m        % pokud je n-té UAV v cíli, vybere se náhodný bod z cílové plochy, kam UAV dorazilo
                s_rand(:,n) = random_state_goal(goal_reached(1,n));
            else                             % pokud n-té UAV není v cíli, vybere se náhodný bod z celé mapy
                s_rand(:,n) = random_state(0, params.world_dimensions(1,1), ...
                    0, params.world_dimensions(2,1));
            end
        end
    else
        ratio = number_of_uavs/length(guiding_paths); % pomìr UAV k poètu vedoucích cest
        for m=1:length(guiding_paths)
            guiding_path = guiding_paths{m};
            best_reached = min(index((m-1)*ratio+1:(m-1)*ratio+ratio,m));
            
%             assignin('base', 'index', index((m-1)*ratio+1:(m-1)*ratio+ratio,m));
%             assignin('base', 'best_reached', best_reached);
            
            center = [guiding_path(best_reached).x; ...
                guiding_path(best_reached).y];
            for n=1:ratio
                if goal_reached(1,(m-1)*ratio+n) == m
                    s_rand(:,(m-1)*ratio+n) = random_state_goal(goal_reached(1,(m-1)*ratio+n));
                else    % náhodná pozice z okruhu okolo nejbližšího bodu nejbližší vedoucí cesty
                    while true
                        s_rand(:,(m-1)*ratio+n) = random_state_polar(center, 0, params.sampling_radius);
   %                     if ~check_inside_obstacle(s_rand(:,(m-1)*ratio+n))
                            break
    %s                    end
                    end
   %                 break
                end
            end
        end
    end
else
    fprintf('%d is invalid Guided path sampling case.', ...
        params.rrt_path_sampling_case);
end
end

