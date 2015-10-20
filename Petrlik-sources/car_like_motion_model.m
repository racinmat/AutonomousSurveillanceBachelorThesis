function [ node, trajectory ] = car_like_motion_model( node, inputs )
%CAR_LIKE_MOTION_MODEL Simple car-like motion model.
%

global number_of_uavs params

%model parameters
%front to rear axle distance
L = params.uav_size;

%simulation parameters
%time step of simulation
time_step = 0.05;

%time of simulation
end_time = 0.5;

%prepare trajectory array
trajectory = repmat(node, 1, ceil(end_time/time_step)) ;

%index of inputs and trajectory arrays
index=1;

%main simulation loop
for t=time_step:time_step:end_time
    for n = 1:number_of_uavs
        %calculate derivatives from inputs
        dx=inputs(1,n)*cos(node.rot(3,n));
        dy=inputs(1,n)*sin(node.rot(3,n));
        dPHI=inputs(1,n)/L*tan(inputs(2,n));
        
        %calculate current state variables
        node.loc(1,n)=node.loc(1,n)+dx*time_step;
        node.loc(2,n)=node.loc(2,n)+dy*time_step;
        node.rot(3,n)=node.rot(3,n)+dPHI*time_step;
        
        node.prev_inputs(:,n) = inputs(:,n);
    end
    %update trajectory with current state
    index=index+1;
    trajectory(index) = node;
end

