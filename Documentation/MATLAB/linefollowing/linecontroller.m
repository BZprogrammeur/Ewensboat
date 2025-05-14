% To generate control inputs for line following
% Author: Dr Jian Wan

function [angle_rudder,angle_sail,q]=linecontroller(x,a,b,r,q,gamma,phi,angle_ruddermax,angle_truewind,speed_truewind)
% x(1) --- x;
% x(2) --- y;
% x(3) --- theta;
% x(4) --- v;
% x(5) --- w;
% a --- the starting point;
% b --- the ending point;
% r --- the cutoff distance;
% q --- the tacking variable;
% gamma --- the incidence angle;
% phi --- the close hauled angle;
% angle_ruddermax --- the maximum rudder angle;
% angle_truewind --- the true wind direction;
% speed_truewind --- the true wind speed;

apparentwindx=speed_truewind*cos(angle_truewind-x(3))-x(4);
apparentwindy=speed_truewind*sin(angle_truewind-x(3));
angle_apparentwind=atan2(apparentwindy,apparentwindx);

m=x(1:1:2,1);
ba=b-a;
c=(b-a)/norm(b-a);
d=m-a;
e=c(1)*d(2)-d(1)*c(2);

if abs(e) > r/2
   q=sign(e);
end
angle_target=atan2(ba(2),ba(1));
angle_nominal=angle_target-2*gamma*atan(e/r)/pi;

if cos(angle_truewind-angle_nominal)+cos(phi) < 0 || (abs(e) < r && cos(angle_truewind-angle_target)+cos(phi) < 0)
   angle_actual=pi+angle_truewind-q*phi;
else
   angle_actual=angle_nominal;
end

if cos(x(3)-angle_actual) >= 0
   angle_rudder=angle_ruddermax*sin(x(3)-angle_actual);
else
   angle_rudder=angle_ruddermax*sign(sin(x(3)-angle_actual));
end
angle_sailmax=pi/2*((cos(angle_truewind-angle_actual)+1)/2);
angle_sail=-sign(angle_apparentwind)*min(abs(pi-abs(angle_apparentwind)),angle_sailmax);
