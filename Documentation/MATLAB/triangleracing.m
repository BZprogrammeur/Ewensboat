% To test the triangle racing algorithm
% Author: Dr Jian Wan

% The initial sailboat state and the line to be followed
angle_truewind=3*pi/2;
waypoints=[0 200 180;0 0 180];
x0=[-20;-20;pi/6;1;0];angle_rudder0=-pi/6;angle_sail0=15/180*pi;

% The environment and sailboat parameters
speed_truewind=2;
rw=10; % the waypoint size
rt=24; % the tacking distance
gamma=pi/4;
phi=pi/3; % the close hauled angle
angle_ruddermax=pi/4;

% To save the animation
video=VideoWriter('triangleracing.avi');
open(video);
set(gca,'nextplot','replacechildren');
drawwaypoints(waypoints,angle_truewind);
hold on
drawsailboat(x0,angle_rudder0,angle_sail0,25);
hold on
frame=getframe(gcf);
writeVideo(video,frame);
clf;

% To specify the simulation time
timestep=1;
timefinal=1100;

% To save the trajectory of the sailboat
X=x0;
Angle_Rudder=angle_rudder0;
Angle_Sail=angle_sail0;
ka=1;
kb=2;
kmax=size(waypoints,2);

% To set the initial tacking variable
q=sign(angle_sail0);

for time=0:timestep:timefinal
    
    [t,x]=ode45(@(t,x)modelsailboat(t,x,angle_rudder0,angle_sail0,angle_truewind,speed_truewind),[time time+timestep],x0);
    x0=x(end,:)';
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % To calculate the desired rudder and sail angles
    r1=norm(x0(1:1:2,1)-waypoints(:,kb));
    if r1 < rw && kb < kmax
       ka=kb;
       kb=kb+1;
    elseif r1 < rw && kb == kmax
       ka=kb;
       kb=1;
    end
    a=waypoints(:,ka);
    b=waypoints(:,kb);
    [angle_rudder0,angle_sail0,q]=linecontroller(x0,a,b,rt,q,gamma,phi,angle_ruddermax,angle_truewind,speed_truewind)
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    X=[X x0];
    Angle_Rudder=[Angle_Rudder angle_rudder0];
    Angle_Sail=[Angle_Sail angle_sail0];
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % To plot the new status
    drawwaypoints(waypoints,angle_truewind);
    hold on
    drawsailboat(x0,angle_rudder0,angle_sail0,25);
    hold on
    n=size(X,2);
    plot(X(1,1:10:n-15),X(2,1:10:n-15),'b.');
    hold on
    frame=getframe(gcf);
    writeVideo(video,frame);
    if time == timefinal
        break
    else
        clf
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   
end

close(video)
