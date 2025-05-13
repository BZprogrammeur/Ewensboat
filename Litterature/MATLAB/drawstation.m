% To draw the field of station keeping with the true wind direction
% Author: Dr Jian Wan

function drawstation(pcenter,diainner,diaouter,angle_truewind)
% pcenter --- the center of the station to be represented by [x;y];
% diainner --- the diameter for the inner circle;
% diaouter --- the diameter for the outer circle;
% angle_truewind --- the angle of the true wind;

pos1=[pcenter'-[diainner/2 diainner/2] diainner diainner];
rectangle('Position',pos1,'Curvature',[1 1],'EdgeColor','r','linewidth',2);
hold on
pos2=[pcenter'-[diaouter/2 diaouter/2] diaouter diaouter];
rectangle('Position',pos2,'Curvature',[1 1],'EdgeColor','b','linewidth',2);
hold on

% To define the size of the field
axis equal
xmin=-30;
xmax=30;
axis([xmin xmax xmin xmax]);

if angle_truewind == 0
   text(xmin+2,xmax-2,'Wind \rightarrow','FontSize',10,'Color','b');
elseif angle_truewind == pi/2
   text(xmin+2,xmax-2,'Wind \uparrow','FontSize',10,'Color','b');
elseif angle_truewind == pi
   text(xmin+2,xmax-2,'Wind \leftarrow','FontSize',10,'Color','b');
elseif angle_truewind == 3*pi/2
   text(xmin+2,xmax-2,'Wind \downarrow','FontSize',10,'Color','b');
end
