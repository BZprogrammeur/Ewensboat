% To test the line following algorithm
% Author: Dr Jian Wan

% The initial sailboat state and the line to be followed
casenumber=6;

if casenumber == 1
       angle_truewind=0;
       ab=[-180 180;0 0];
       x0=[-150;50;-pi/6;1;0];angle_rudder0=pi/6;angle_sail0=-pi/3;
elseif casenumber == 2
       angle_truewind=pi/2;
       ab=[-180 180;-180 180];
       x0=[-100;-160;pi/6;1;0];angle_rudder0=pi/6;angle_sail0=-pi/3;
elseif casenumber == 3
       angle_truewind=pi;
       ab=[-180 180;0 0];
       x0=[-150;50;-pi/3;1;0];angle_rudder0=pi/6;angle_sail0=-15/180*pi;
elseif casenumber == 4
       angle_truewind=3*pi/2;
       ab=[-180 180;-180 180];
       x0=[-150;-150;0;1;0];angle_rudder0=pi/6;angle_sail0=pi/4;
elseif casenumber == 5
       angle_truewind = pi/4;
       ab = [-180 180;-180 180];
       x0=[-150;-150;0;1;0];angle_rudder0=pi/6;angle_sail0=pi/4;
elseif casenumber == 6
       angle_truewind = -3*pi/4;
       ab = [-180 180;-180 180];
       x0=[-150;-150;0;1;0];angle_rudder0=pi/6;angle_sail0=pi/4;
end

% The environment and sailboat parameters
speed_truewind=2;
r=40; % the curoff distance
phi=pi/3; % the close hauled angle
angle_ruddermax=pi/4;
gamma=pi/4;

% To save the animation
video=VideoWriter('linefollowing.avi');
open(video);
set(gca,'nextplot','replacechildren');
drawlinepath(ab,r,angle_truewind);
hold on
drawsailboat(x0,angle_rudder0,angle_sail0,25);
hold on
frame=getframe(gcf);
writeVideo(video,frame);
clf;

% To specify the simulation time
timestep=1;
timefinal=600;

% To save the trajectory of the sailboat
X=x0;
Angle_Rudder=angle_rudder0;
Angle_Sail=angle_sail0;

% To set the initial tacking variable
q=sign(angle_sail0);

for time=0:timestep:timefinal
    
    [t,x]=ode45(@(t,x)modelsailboat(t,x,angle_rudder0,angle_sail0,angle_truewind,speed_truewind),[time time+timestep],x0);
    x0=x(end,:)';
    
    a=ab(:,1);
    b=ab(:,2);
    [angle_rudder0,angle_sail0,q]=linecontroller(x0,a,b,r,q,gamma,phi,angle_ruddermax,angle_truewind,speed_truewind);
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    X=[X x0];
    Angle_Rudder=[Angle_Rudder angle_rudder0];
    Angle_Sail=[Angle_Sail angle_sail0];
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % To plot the new status
    drawlinepath(ab,r,angle_truewind);
    hold on
    drawsailboat(x0,angle_rudder0,angle_sail0,25);
    hold on
    n=size(X,2);
    plot(X(1,1:10:n-15),X(2,1:10:n-15),'b.');
    hold on
    frame=getframe(gcf);
    writeVideo(video,frame);
    clf
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
end
close(video)