% To draw the field of triangle racing with the true wind direction
% Author: Dr Jian Wan

function drawwaypoints(waypoints,angle_truewind)
% waypoints --- the waypoints to be followed and to be represented by [x1 x2 x3 ...;y1 y2 y3 ...];
% angle_truewind --- the true wind direction;

n=size(waypoints,2);

for i=1:1:n
    plot(waypoints(1,i),waypoints(2,i),'ko');
    hold on
    if i < n
       plot(waypoints(1,i:1:i+1),waypoints(2,i:1:i+1),'-.','color','k');
       hold on
    else
       plot(waypoints(1,[i 1]),waypoints(2,[i 1]),'-.','color','k');
    end
end

% To define the size of the field
axis equal
xmin=-50;
xmax=250;
axis([xmin xmax xmin xmax]);

if angle_truewind == 0
   text(xmin+10,xmax-20,'Wind \rightarrow','FontSize',10,'Color','b');
elseif angle_truewind == pi/2
   text(xmin+10,xmax-20,'Wind \uparrow','FontSize',10,'Color','b');
elseif angle_truewind == pi
   text(xmin+10,xmax-20,'Wind \leftarrow','FontSize',10,'Color','b');
elseif angle_truewind == 3*pi/2
   text(xmin+10,xmax-20,'Wind \downarrow','FontSize',10,'Color','b');
end
