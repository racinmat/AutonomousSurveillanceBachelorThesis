function [  ] = prepare_maps(  )
%PREPARE_MAPS Summary of this function goes here
%   Detailed explanation goes here

global number_of_uavs maps

%Maze
for m = 1:number_of_uavs
    maps(1,1).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,1).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,1).goals = ...
    {Goal('rectangle', 651, 701, 348, 148)};

maps(1,1).obstacles = ...
    {Obstacle(0, 400, 700, 50) ...
    Obstacle(250, 600, 750, 50) ...
    Obstacle(400, 0, 50, 200) ...
    Obstacle(200, 600, 50, 200) ...
    Obstacle(650, 200, 50, 200)};


%Two goals
for m = 1:number_of_uavs
    maps(1,2).init_node.loc(1:3,m) = [401+m*30;501+m*20*0;0];
    maps(1,2).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,2).goals = ...
    {Goal('rectangle', 1, 426, 149, 148) ...
    Goal('rectangle', 851, 426, 149, 148)};

maps(1,2).obstacles = ...
    {Obstacle(151, 299, 99, 401) ...
    Obstacle(751, 299, 99, 401) ...
    Obstacle(451, 599, 99, 401) ...
    Obstacle(451, 0, 99, 401)};


%Company campus
for m = 1:number_of_uavs
    maps(1,3).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,3).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,3).goals = ...
    {Goal('rectangle',651,850,230,100) ...
    Goal('rectangle',651,550,230,100)};

maps(1,3).obstacles = ...
    {Obstacle(600,650,350,200) ...
    ...
    Obstacle(49,600,400,200) ...
    ...
    Obstacle(200,100,200,400)...
    Obstacle(100,400,100,100)...
    Obstacle(400,400,100,100)...
    ...
    Obstacle(600,100,300,100)...
    Obstacle(700,200,200,299)};


%Simple map
for m = 1:number_of_uavs
    maps(1,4).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,4).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,4).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,4).obstacles = ...
    {Obstacle(300,300,200,400)};


%Large AoI
for m = 1:number_of_uavs
    maps(1,5).init_node.loc(1:3,m) = [380+m*30;50+m*10*0;0];
    maps(1,5).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,5).goals = ...
    {Goal('rectangle',301,500,398,400)};

maps(1,5).obstacles = ...
    {Obstacle(300,300,400,200)};


%3 goals 2 obstacles
for m = 1:number_of_uavs
    maps(1,6).init_node.loc(1:3,m) = [400+m*30;50+m*10*0;0];
    maps(1,6).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,6).goals = ...
    {Goal('rectangle',385,801,230,100)...
    Goal('rectangle',301,521,150,180)...
    Goal('rectangle',551,521,150,180)...
    };

maps(1,6).obstacles = ...
    {Obstacle(199,699,601,101) ...
    Obstacle(199,519,101,201) ...
    Obstacle(699,519,101,201) ...
    Obstacle(199,199,601,101)};

%Narrow Passage
for m = 1:number_of_uavs
    maps(1,7).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,7).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,7).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,7).obstacles = ...
    {Obstacle(400,0,200,470) ...
    Obstacle(400,480,200,520)};



for m = 1:number_of_uavs
    maps(1,71).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,71).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,71).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,71).obstacles = ...
    {Obstacle(400,0,200,470) ...
    Obstacle(400,480,200,520)};

for m = 1:number_of_uavs
    maps(1,72).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,72).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,72).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,72).obstacles = ...
    {Obstacle(400,0,200,465) ...
    Obstacle(400,485,200,515)};

for m = 1:number_of_uavs
    maps(1,73).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,73).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,73).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,73).obstacles = ...
    {Obstacle(400,0,200,460) ...
    Obstacle(400,490,200,510)};

for m = 1:number_of_uavs
    maps(1,74).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,74).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,74).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,74).obstacles = ...
    {Obstacle(400,0,200,455) ...
    Obstacle(400,495,200,505)};


for m = 1:number_of_uavs
    maps(1,75).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,75).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,75).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,75).obstacles = ...
    {Obstacle(400,0,200,450) ...
    Obstacle(400,500,200,500)};


for m = 1:number_of_uavs
    maps(1,76).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,76).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,76).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,76).obstacles = ...
    {Obstacle(400,0,200,425) ...
    Obstacle(400,525,200,475)};

for m = 1:number_of_uavs
    maps(1,76).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,76).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,76).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,76).obstacles = ...
    {Obstacle(400,0,200,425) ...
    Obstacle(400,525,200,475)};

for m = 1:number_of_uavs
    maps(1,77).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,77).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,77).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,77).obstacles = ...
    {Obstacle(400,0,200,400) ...
    Obstacle(400,550,200,450)};

for m = 1:number_of_uavs
    maps(1,78).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,78).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,78).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,78).obstacles = ...
    {Obstacle(400,0,200,375) ...
    Obstacle(400,575,200,425)};


for m = 1:number_of_uavs
    maps(1,81).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,81).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,81).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,81).obstacles = ...
    {Obstacle(475,0,50,465) ...
    Obstacle(475,485,50,515)};

for m = 1:number_of_uavs
    maps(1,82).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,82).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,82).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,82).obstacles = ...
    {Obstacle(450,0,100,465) ...
    Obstacle(450,485,100,515)};

for m = 1:number_of_uavs
    maps(1,83).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,83).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,83).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,83).obstacles = ...
    {Obstacle(450,0,150,465) ...
    Obstacle(450,485,150,515)};

for m = 1:number_of_uavs
    maps(1,84).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,84).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,84).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,84).obstacles = ...
    {Obstacle(425,0,200,465) ...
    Obstacle(425,485,200,515)};

for m = 1:number_of_uavs
    maps(1,85).init_node.loc(1:3,m) = [50+m*30;50+m*10*0;0];
    maps(1,85).init_node.rot(1:3,m) = [0;0;pi/2];
end

maps(1,85).goals = ...
    {Goal('rectangle',701,701,150,150)};

maps(1,85).obstacles = ...
    {Obstacle(400,0,250,465) ...
    Obstacle(400,485,250,515)};


end


