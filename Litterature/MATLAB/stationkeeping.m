% To test the station keeping algorithm
% Author: Dr Jian Wan

% The initial sailboat state and the line to be followed
angle_truewind=pi/2;
pcenter=[0;10];
x0=[-10;-30;pi/6;1;0];angle_rudder0=0;angle_sail0=2*pi/3;
% x0=[-25;-25;pi/6;1;0];angle_rudder0=0;angle_sail0=pi/6;

% The environment and sailboat parameters
speed_truewind=1;
diainner=14; % 14
diaouter=28; % 28
rt=6; % the tacking distance
gamma=pi/4;
phi=pi/3; % the close hauled angle
angle_ruddermax=pi/4;

% To save the animation
video=VideoWriter('stationkeeping.avi');
open(video);
set(gca,'nextplot','replacechildren');
drawstation(pcenter,diainner,diaouter,angle_truewind);
hold on
drawsailboat(x0,angle_rudder0,angle_sail0,5);
hold on
frame=getframe(gcf);
writeVideo(video,frame);
clf;

% To specify the simulation time
timestep=1;
timefinal=120;

% To save the trajectory of the sailboat
X=x0;
Angle_Rudder=angle_rudder0;
Angle_Sail=angle_sail0;

% To set the initial tacking variable
q=sign(angle_sail0);

for time=0:timestep:timefinal
    
    [t,x]=ode45(@(t,x)modelsailboat(t,x,angle_rudder0,angle_sail0,angle_truewind,speed_truewind),[time time+timestep],x0);
    x0=x(end,:)';
    apparentwindx=speed_truewind*cos(angle_truewind-x0(3))-x0(4);
    apparentwindy=speed_truewind*sin(angle_truewind-x0(3));
    angle_apparentwind=atan2(apparentwindy,apparentwindx);
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % To calculate the desired rudder and sail angles
    a=x0(1:1:2,1);
    b=pcenter;
    ba=b-a;
    angle_target=atan2(ba(2),ba(1));
    angle_actual=angle_target;
    distancetotarget=norm(b-a);
    
    if distancetotarget > diaouter/2 % Step 1
       [angle_rudder0,angle_sail0,q]=linecontroller(x0,a,b,rt,q,gamma,phi,angle_ruddermax,angle_truewind,speed_truewind);
    end
       
    if (distancetotarget > diainner/2 && distancetotarget <= diaouter/2) % Step 2 and Step 3
        
        if cos(angle_target-angle_truewind) > 0
           if cos(pi+angle_target-pi/2-angle_truewind) > cos(pi+angle_target+pi/2-angle_truewind)
              angle_target=angle_target+pi/2;
           else
              angle_target=angle_target-pi/2;
           end
        end
        
        if cos(angle_truewind-angle_target)+cos(phi) < 0
           if sin(angle_truewind-angle_target) > 0
              q=-1;
           else
              q=1;
           end
           angle_actual=angle_truewind+pi+q*phi;
        else
           angle_actual=angle_target;
        end
        
        if cos(x0(3)-angle_actual) > 0
           angle_rudder0=angle_ruddermax*sin(x0(3)-angle_actual);
        else
           angle_rudder0=angle_ruddermax*sign(sin(x0(3)-angle_actual));
        end
        
        angle_sailmax=pi/2*((cos(angle_truewind-angle_actual)+1)/2);
        angle_sail0=-sign(angle_apparentwind)*min(abs(pi-abs(angle_apparentwind)),angle_sailmax); 
    end
    
    if distancetotarget <= diainner/2
       if cos(angle_truewind-angle_target)+cos(phi) > 0
          if cos(angle_target-angle_truewind-pi+phi) > cos(angle_target-angle_truewind-pi-phi)
             q=-1;
          else
             q=1;
          end
          angle_actual=angle_truewind+pi+q*phi;
       else 
          angle_actual=angle_target;
       end
            
       if cos(x0(3)-angle_actual) > 0
          angle_rudder0=angle_ruddermax*sin(x0(3)-angle_actual);
       else
          angle_rudder0=angle_ruddermax*sign(sin(x0(3)-angle_actual));
       end
       
       angle_sailmax=pi/2*((cos(angle_truewind-angle_actual)+1)/2);
       angle_sail0=-sign(angle_apparentwind)*min(abs(pi-abs(angle_apparentwind)),angle_sailmax);    
    end
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    X=[X x0];
    Angle_Rudder=[Angle_Rudder angle_rudder0];
    Angle_Sail=[Angle_Sail angle_sail0];
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % To plot the new status
    drawstation(pcenter,diainner,diaouter,angle_truewind);
    hold on
    drawsailboat(x0,angle_rudder0,angle_sail0,5);
    hold on
    n=size(X,2);
    plot(X(1,1:2:n-5),X(2,1:2:n-5),'b.');
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
