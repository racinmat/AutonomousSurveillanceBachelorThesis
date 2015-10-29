function [ final_node_index, particles, best_particle, trajectory, output ] = pso( pso_bounds, init_node )
%PSO Summary of this function goes here
%   Detailed explanation goes here

global params number_of_uavs goal_map empty_node stop

disp('PSO starting...');
tic
distances = prepare_distances();
[particles, best_particle, best_particles ] = init_particles(pso_bounds, init_node);

best_cfp = Inf(1,params.pso_particles);
best_cfg = Inf;
values = [];
trajectory(1,1:params.pso_particles) = init_node;
goal_reached = NaN(1,number_of_uavs);


%% PSO Iteration loop
for m=2:params.pso_iter
    if stop
        return
    end
    fprintf('PSO iteration %d starting...\n',m);
    tv = tic;
    particles_stuck = 0;
    particles(m,:) = repmat(empty_node, 1, params.pso_particles);
    inertia = decrease_inertia(m);
    
    %PSO loop through particles
    for k=1:params.pso_particles
        
        %PSO loop through uavs
        for n=1:number_of_uavs
            
            %Current PSO node velocity
            particles(m,k).v(1,n) = ...
                inertia*particles(m-1,k).v(1,n) + ...
                params.pso_phi_p*rand()*(best_particles(1,k).loc(1,n)-particles(m-1,k).loc(1,n)) + ...
                params.pso_phi_g*rand()*(best_particle.loc(1,n)-particles(m-1,k).loc(1,n));
            particles(m,k).v(2,n) = ...
                inertia*particles(m-1,k).v(2,n) + ...
                params.pso_phi_p*rand()*(best_particles(1,k).loc(2,n)-particles(m-1,k).loc(2,n)) + ...
                params.pso_phi_g*rand()*(best_particle.loc(2,n)-particles(m-1,k).loc(2,n));
            particles(m,k).v(3,n) = 0;
        end
        
        %Current PSO node correction
        particles(m,k) = check_velocity_bounds(particles(m,k), particles(m-1,k), pso_bounds);
        particles(m,k) = relative_localization(particles(m,k), particles(m-1,k));
        
        %Location of new PSO node from previous node location and
        %velocity
        for n=1:number_of_uavs
            particles(m,k).loc(1,n) = particles(m-1,k).loc(1,n) + particles(m,k).v(1,n);
            particles(m,k).loc(2,n) = particles(m-1,k).loc(2,n) + particles(m,k).v(2,n);
            particles(m,k).loc(3,n) = 0;
            
            particles(m,k).rot(3,n) = particles(m-1,k).rot(3,n) ...
                + atan2(particles(m,k).v(2,n), particles(m,k).v(1,n));
        end
        
        
 %       [~,~,uavs_colliding]=check_obstacle_vcollide(particles(m,k),zeros(3,number_of_uavs));
        
%         if any(uavs_colliding)
%             particles(m,k) = particles(m-1,k);
%             particles_stuck = particles_stuck + 1;
%             fprintf('Particle %d stuck in %d iteration of PSO. (obstacle collision)\n',k,m);
%             for n=1:number_of_uavs
%                 for l=1:2
%                     maxv = params.pso_vmax*(pso_bounds(l,2)-pso_bounds(l,1));
%                     particles(m,k).v(l,n) = -maxv + rand()*maxv*2 ...
%                         + params.pso_phi_p*rand()*(best_particles(1,k).loc(1,n)-particles(m-1,k).loc(1,n)) ...
%                         + params.pso_phi_g*rand()*(best_particle.loc(1,n)-particles(m-1,k).loc(1,n));
%                 end
%             end
%             
%         end
        
        %PSO state space bounds check
        if any(particles(m,k).loc(1,:)>pso_bounds(1,2)) || ...
                any(particles(m,k).loc(1,:)<pso_bounds(1,1)) || ...
                any(particles(m,k).loc(2,:)>pso_bounds(2,2)) || ...
                any(particles(m,k).loc(2,:)<pso_bounds(2,1))
            particles(m,k) = particles(m-2,k);
            particles_stuck = particles_stuck + 1;
            fprintf('Particle %d stuck in %d iteration of PSO. (space bounds collision)\n',k,m);
%             for n=1:number_of_uavs
%                 for l=1:2
%                     maxv = params.pso_vmax*(pso_bounds(l,2)-pso_bounds(l,1));
%                     particles(m,k).v(l,n) = -maxv + rand()*maxv*2 ...
%                         + params.pso_phi_p*rand()*(best_particles(1,k).loc(1,n)-particles(m-1,k).loc(1,n)) ...
%                         + params.pso_phi_g*rand()*(best_particle.loc(1,n)-particles(m-1,k).loc(1,n));
%                 end
%             end
            continue
        end
        
        %Motion model
        [particles(m-1,k), tmp_particle] = ...
            select_input(particles(m,k).loc, particles(m-1,k));
        tmp_particle.v = particles(m,k).v;
        particles(m,k) = tmp_particle;
        if any(isnan(particles(m,k).loc(1,:)))
            particles_stuck = particles_stuck + 1;
            if particles_stuck>=params.pso_particles
                disp('All particles stuck.');
            end
            particles(m,k) = particles(m-2,k);
            
            fprintf('Particle %d stuck in %d iteration of PSO. (no valid input)\n',k,m);
%             for n=1:number_of_uavs
%                 for l=1:2
%                     maxv = params.pso_vmax*(pso_bounds(l,2)-pso_bounds(l,1));
%                     particles(m,k).v(l,n) = -maxv + rand()*maxv*2;
                     particle(m,k).used_inputs(:) = false;
%                     particles(m,k).rot(3,n) = particles(m-1,k).rot(3,n) ...
%                 + atan2(particles(m,k).v(2,n), particles(m,k).v(1,n));
%                 end
%             end
%              particles(m,k) = check_velocity_bounds(particles(m,k), particles(m-1,k), pso_bounds);
        end
        
        %Current PSO node indexing
%         particles(m,k).prev = particles(m-1,k).index;
        particles(m,k).index = particles(m,k).prev + 1;
%         particles(m,k).index = m;
         particles(m,k).prev = particles(m,k).index - 1;
        %Visualization of every particle
        if params.visualize && params.pso_visualize_all
            visualize_growth(particles(m-1,k).loc(1:2,:), ...
                particles(m,k).loc(1:2,:),particles(m,k).loc(1:2,:));
        end
        
        %Evaluate PSO Cost Functions values
        cf = fitness_function(particles(m,k).loc);
        cf_dist(m,k) = distance_cf(particles(m,k).loc, distances); %#ok
        cf = cf + 0.001*cf_dist(m,k);
        goal_map.reset();
        
        %Best Cost Function of current particle
        if cf<best_cfp(1,k)
            best_cfp(1,k) = cf;
            best_particles(1,k) = particles(m,k);
            
            %Best global Cost Function
            if cf<best_cfg
                best_cfg = cf;
                best_particle = particles(m,k);
                final_node_index = [m k];
                
                %Visualization
                if params.visualize && ~params.pso_visualize_all
                    visualize_growth(particles(m-1,k).loc(1:2,:), ...
                        particles(m,k).loc(1:2,:),particles(m,k).loc(1:2,:));
                end
                
            end
        end
        
        dt = toc(tv);
        output.time_to_find_new_node(1,m) = dt;
        output.iterations(1,m) = m;
        try
            output.time_running(1,m) = output.time_running(1,m-1) + dt;
        catch
            output.time_running(1,m) = dt;
        end
        
        if output.time_running(1,m) > params.max_iteration_time
            return
        end
    end
    
    %Cost Function values of best particle through iterations
    values(1,end+1) = best_cfg; %#ok
    
    %Debug output of stuck particles
    if params.debug
        fprintf('Particles stuck in %d. iteration: %d/%d\n', ...
            m, particles_stuck, params.pso_particles);
    end
    
    %End if all particles stuck
    if particles_stuck == params.pso_particles
        %      break
    end
    
    goal_reached = check_near_goal(best_particle.loc);
    output.goal_reached = goal_reached;
end
fprintf('PSO took %f seconds.\n', toc);
end


%% Particles initialization
function [ particles, best_particle, best_particles ] = init_particles( pso_bounds, init_node )

global params empty_node max_t

max_t = 0;
particles = repmat(empty_node, 1, params.pso_particles);
for k=1:params.pso_particles
    particles(1,k) = init_particle(pso_bounds, init_node);
    best_particles(1,k) = get_best_particle(particles); %#ok
end
best_particle = get_best_particle(particles);

end


%% Individual particle initialization
function [ particle ] = init_particle( pso_bounds, init_node )

global number_of_uavs goal_map params

particle = init_node;
for n=1:number_of_uavs
    for m=1:2
        maxv = params.pso_vmax*(pso_bounds(m,2)-pso_bounds(m,1));
        particle.v(m,n) = -maxv + rand()*maxv*2;
    end
end

particle.best_cfp = fitness_function(particle.loc);
goal_map.reset();
end


%% Find particle with best cost function
function [ best_particle ] = get_best_particle( particles )

global params

best_cfg = Inf;
for k=1:params.pso_particles
    if particles(1,k).best_cfp < best_cfg
        best_cfg = particles(1,k).best_cfp;
        best_particle = particles(1,k);
    end
end
end

%% Cost Function according to distance to goal
function [cf_distance] = distance_cf(loc, distances)

global params number_of_uavs goals

for m=1:length(goals)
    for n=1:number_of_uavs
        cf(m,n) = Inf; %#ok
        for k=1:length(distances{m})
            if loc(1,n)>=distances{m}(1,k).x && loc(1,n)<=distances{m}(1,k).x+params.cell_size ...
                    && loc(2,n)>=distances{m}(1,k).y && loc(2,n)<=distances{m}(1,k).y+params.cell_size
                cf(m,n) = distances{m}(1,k).g_score;
                break
            end
        end
    end
    
    tmp_distance(1,m) = sum(cf(m,:)); %#ok
end
cf_distance = sum(tmp_distance);
end


%% Decrease inertia of particle
function [ inertia ] = decrease_inertia(current_iter)
global params
inertia = params.pso_inertia_start + ...
    current_iter*(params.pso_inertia_end-params.pso_inertia_start)/params.pso_iter;
end


%% Lower speed to keep particle in bounds
function [ particle ] = check_velocity_bounds( particle, prev_particle, pso_bounds )

global number_of_uavs params max_t

t=tic;
for n=1:number_of_uavs
    for m=1:2
        stuck_count = 1;
        
        %Check max speed
        vmax = params.pso_vmax.*(pso_bounds(:,2)-pso_bounds(:,1));
        if particle.v(m,n)>vmax(m,1)
            particle.v(m,n) = vmax(m,1);
        elseif particle.v(m,n)<-vmax(m,1)
            particle.v(m,n) = -vmax(m,1);
        end
        
        %Check collisions with borders
        while (prev_particle.loc(m,n)+particle.v(m,n)) > pso_bounds(m,2) || ...
                (prev_particle.loc(m,n)+particle.v(m,n)) < pso_bounds(m,1)
            particle.v(m,n) = particle.v(m,n).*0.75;
            stuck_count = stuck_count +1;
            if stuck_count > 50
                disp('Out of bounds');
                break
            end
        end
    end
end

%Check collisions with obstacles
for n=1:number_of_uavs
    [ ~, ~, uavs_colliding ] = check_obstacle_vcollide( prev_particle, ...
        particle.v);
    v_start = particle.v(:,n);
    dim = 1;
    stuck_count = 0;
    while(uavs_colliding(1,n))
        particle = relative_localization( particle, prev_particle );
        if ~uavs_colliding(1,n)
            particle = relative_localization( particle, prev_particle );
            break
        end
        for o=1:5
            particle.v(dim,n) = particle.v(dim,n)*0.5;
            [ ~, ~, uavs_colliding ] = check_obstacle_vcollide( prev_particle, ...
                particle.v);
            if ~uavs_colliding(1,n)
                particle = relative_localization( particle, prev_particle );
                break
            end
        end
        if ~uavs_colliding(1,n)
            particle = relative_localization( particle, prev_particle );
            break
        end
        
        if dim == 2
            particle.v(:,n) = v_start(:,1);
            particle.v(:,n) = particle.v(:,n)*0.5;
            v_start(:,n) = particle.v(:,n)*0.5;
            dim = 1:2;
            continue
        elseif all(dim == 1:2)
            particle.v(:,n) = -v_start(:,1);
            stuck_count = stuck_count + 1;
            if stuck_count > 5
                particle.v(:,n) = [-vmax(1,1)+rand()*vmax(1,1)*2; ...
                    -vmax(2,1)+rand()*vmax(2,1)*2; 0];
                particle = relative_localization( particle, prev_particle );
                continue
            end
            continue
        else
            dim = dim + 1;
            particle.v(:,n) = v_start(:,1);
            continue
        end
        
    end
    
    
end
t=toc(t);
if t>max_t
    max_t = t;
    if params.debug
        fprintf('Non colliding velocity found in %d\n',max_t);
    end
end
end


%% Modify speed to satisfy relative localization constraints
function [ particle ] = relative_localization( particle, prev_particle )

global number_of_uavs params

tmp_particle = particle;
tmp_particle.loc(:,:) = prev_particle.loc(:,:) + particle.v(:,:);

if check_localization_sep(tmp_particle)
    return
end

v_samples = zeros(3,number_of_uavs,params.pso_v_samples+2);
n = number_of_uavs;
while n>0
    v_mean = [(particle.v(1,n)+particle.v(1,n-1))/2; ...
        (particle.v(2,n)+particle.v(2,n-1))/2];
    v_samples(:,n-1:n,1) = particle.v(:,n-1:n);
    for k=2:params.pso_v_samples
        v_samples(1:2,n,k) = (v_samples(1:2,n,k-1)+v_mean(1:2))./2;
        v_samples(1:2,n-1,k) = (v_samples(1:2,n-1,k-1)+v_mean(1:2))./2;
        if mod(n,2)~=0
            disp('Only even count of UAVs supported');
            return
        end
    end
    v_samples(:,n-1:n,params.pso_v_samples+1) = [v_mean v_mean; 0 0];
    v_samples(:,n-1:n,params.pso_v_samples+2) = zeros(3,2,1);
    n = n-2;
end

k = 1;
while ~check_localization_sep(tmp_particle) && k < length(v_samples)
    particle.v = v_samples(:,:,k);
    tmp_particle = particle;
    tmp_particle.loc(:,:) = prev_particle.loc(:,:) + particle.v(:,:);
    k = k + 1;
end
end

%% Prepare distances from cells to goal
function [ distances ] = prepare_distances

global goals

for m=1:length(goals)
    map_grid = get_map_grid(m);
    grid_nodes = grid_to_nodes( map_grid );
    [i, j] =  find(map_grid == 3, 1);
    p2 = i*size(map_grid,2)+j;
    dist = distance_transform(p2, grid_nodes);
    distances{m} = dist; %#ok
end
end