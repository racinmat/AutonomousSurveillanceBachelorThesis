function [ node, trajectory ] = car_like_motion_model( node, inputs )
%CAR_LIKE_MOTION_MODEL Simple car-like motion model.
%

global number_of_uavs params empty_trajectory

%model parameters
%front to rear axle distance
L = params.uav_size;

%prepare trajectory array
%repmat is time demanding
% trajectory = repmat(node, 1, params.simulation_steps);
trajectory = empty_trajectory;

%index of inputs and trajectory arrays
index=0;

%main simulation loop
for t=params.time_step:params.time_step:params.end_time
    for n = 1:number_of_uavs
        %calculate derivatives from inputs
        dx=inputs(1,n)*cos(node.rot(3,n));
        dy=inputs(1,n)*sin(node.rot(3,n));
        dPHI=inputs(1,n)/L*tan(inputs(2,n));
        
        %calculate current state variables
        node.loc(1,n)=node.loc(1,n)+dx*params.time_step;
        node.loc(2,n)=node.loc(2,n)+dy*params.time_step;
        node.rot(3,n)=node.rot(3,n)+dPHI*params.time_step;
        
        
    end
    node.prev_inputs = inputs;
    
    %update trajectory with current state
    index=index+1;
    trajectory(index) = node;
end

