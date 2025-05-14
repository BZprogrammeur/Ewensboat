% To draw the field of line following with the true wind direction
% Author: Dr Jian Wan

function drawlinepath(ab,r,angle_truewind)
% ab --- the line to be followed and to be represented by [x1 x2;y1 y2];
% r --- the cutoff distance;
% angle_truewind --- the angle of the true wind;

abupper=ab+[0 0;r/2 r/2];
ablower=ab-[0 0;r/2 r/2];

plot(ab(1,:),ab(2,:),'k');
hold on
plot(abupper(1,:),abupper(2,:),'g-.');
hold on
plot(ablower(1,:),ablower(2,:),'g-.');
hold on

% To define the size of the field
axis equal
xmin=-180;
xmax=180;
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
