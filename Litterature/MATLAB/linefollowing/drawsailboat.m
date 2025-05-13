% To draw a sailboat with rudder and sail angles
% Author: Dr Jian Wan

function drawsailboat(x,angle_rudder,angle_sail,sizefactor)
% x(1) --- x;
% x(2) --- y;
% x(3) --- theta;
% x(4) --- v;
% x(5) --- w;
% angle_rudder --- the rudder angle;
% angle_sail --- the sail angle;

magnifyfactor=sizefactor; % to magnify the size of the drawed sailboat

theta=x(3);
rotation_matrix=[cos(theta) -sin(theta);sin(theta) cos(theta)];
new_center=[x(1);x(2)];

hull=magnifyfactor*[-0.5 -0.5 0.15 0.5 0.15 -0.5; -0.15 0.15 0.15 0 -0.15 -0.15];
hull=rotation_matrix*hull+repmat(new_center,1,size(hull,2));

rudder=magnifyfactor*[-0.5 -0.2*cos(angle_rudder)-0.5;0 -0.2*sin(angle_rudder)];
rudder=rotation_matrix*rudder+repmat(new_center,1,size(rudder,2));

sail=magnifyfactor*[0.05 0.05-0.4*cos(angle_sail);0 -0.4*sin(angle_sail)];
sail=rotation_matrix*sail+repmat(new_center,1,size(sail,2));

central_line=magnifyfactor*[-0.5 0.5;0 0];
central_line=rotation_matrix*central_line+repmat(new_center,1,size(central_line,2));

plot(hull(1,:),hull(2,:),'k');
hold on
plot(rudder(1,:),rudder(2,:),'r')
hold on
plot(sail(1,:),sail(2,:),'r')
hold on
plot(central_line(1,:),central_line(2,:),'b-.')
hold on
