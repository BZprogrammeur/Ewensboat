% To provide the state-space model for a sailboat
% Author: Dr Jian Wan

function dxdt=modelsailboat(t,x,angle_rudder,angle_sail,angle_truewind,speed_truewind)
% t --- the time
% x(1) --- x;
% x(2) --- y;
% x(3) --- theta;
% x(4) --- v;
% x(5) --- w;
% angle_rudder --- the rudder angle;
% angle_sail --- the sail angle;
% angle_truewind --- the true wind direction;
% speed_truewind --- the true wind speed;

% p1=0.125; p2=29.99; p3=96.43; p4=58.07; p5=120.65; p6=0.1; p7=0; p8=0.5; p9=10; p10=29.87; p11=0; % Plymouth
% p1=0.05; p2=0.2; p3=6000; p4=1000; p5=2000; p6=1; p7=1; p8=2; p9=300; p10=10000; p11=1; % Vamos
p1=0.03; p2=40; p3=6000; p4=200; p5=1500; p6=0.5; p7=0.5; p8=2; p9=300; p10=400; p11=0.2; % Aland
speed_apparentwind=sqrt((speed_truewind*cos(angle_truewind-x(3))-x(4))^2+(speed_truewind*sin(angle_truewind-x(3)))^2);
angle_apparentwind=atan2((speed_truewind*sin(angle_truewind-x(3))),(speed_truewind*cos(angle_truewind-x(3))-x(4)));

gs=p4*speed_apparentwind*sin(angle_sail-angle_apparentwind);
gr=p5*x(4)^2*sin(angle_rudder);

dxdt=zeros(5,1);
dxdt(1)=x(4)*cos(x(3))+p1*speed_truewind*cos(angle_truewind);
dxdt(2)=x(4)*sin(x(3))+p1*speed_truewind*sin(angle_truewind);
dxdt(3)=x(5);
dxdt(4)=(gs*sin(angle_sail)-gr*p11*sin(angle_rudder)-p2*x(4)^2)/p9;
dxdt(5)=(gs*(p6-p7*cos(angle_sail))-gr*p8*cos(angle_rudder)-p3*x(5)*x(4))/p10;
