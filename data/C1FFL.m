% COHERENT FEED FORWARD LOOP

clear all;

% constants
TIME_STEP = 0.01;
tmax = 2;
timeArray = 0:TIME_STEP:tmax;
n = length(timeArray);

% network
xInd=1; yInd=2; zInd=3;
productionRate = [30,17,25];
degradationRate = [2,1,5];
steadyState(xInd) = productionRate(xInd)/degradationRate(xInd);
steadyState(yInd) = productionRate(yInd)/degradationRate(yInd);
steadyState(zInd) = productionRate(zInd)/degradationRate(zInd);
threshold_xy = 10;
threshold_yz = 11;
threshold_xz = 13;

% preconditions
x0 = 0;
y0 = 0;
z0 = 0;

% initializing
x = zeros(1,n);
y = zeros(1,n);
z = zeros(1,n);
x(1) = x0; 
y(1) = y0; 
z(1) = z0;

ex_y = zeros(1,n);
ex_y(1) = y0;
ex_z = zeros(1,n);
ex_z(1) = z0;

tau = [0,0,0];
active = [true,false,false];

ex_x = steadyState(xInd)*(1-exp(-degradationRate(xInd)*timeArray));

 for tInd = 2:n,
  % approximation
  dx = productionRate(xInd)-degradationRate(xInd)*x(tInd-1);
  x(tInd) = x(tInd-1) + dx*TIME_STEP;

  level_y = activityLevel(x(tInd-1),threshold_xy);
  dy = (level_y*productionRate(yInd))-degradationRate(yInd)*y(tInd-1);
  y(tInd) = y(tInd-1) + dy*TIME_STEP;
  
  level_xz = activityLevel(x(tInd-1),threshold_xz);
  level_yz = activityLevel(y(tInd-1),threshold_yz);
  level_z = 0.5*level_xz + 0.5*level_yz;
  dz = (level_z*productionRate(zInd))-degradationRate(zInd)*z(tInd-1);
  z(tInd) = z(tInd-1) + dz*TIME_STEP;

   % simulation
  if level_y>0.01 && active(yInd)==false,
      active(yInd) = true;
      tau(yInd) = timeArray(tInd-1);
  end;
  localTime_y = timeArray(tInd) - tau(yInd);
  ex_y(tInd) = level_y*steadyState(yInd)*(1-exp(-degradationRate(yInd)*localTime_y));
  
  if level_z>0.01 && active(zInd)==false,
      active(zInd)=true;
      tau(zInd) = timeArray(tInd-1);
  end;
  localTime_z = timeArray(tInd) - tau(zInd);
  ex_z(tInd) = level_z*steadyState(zInd)*(1-exp(-degradationRate(zInd)*localTime_z));

 end; 

subplot(311); plot(timeArray,[ex_x; x]);
subplot(312); plot(timeArray,[ex_y; y]);
subplot(313); plot(timeArray,[ex_z; z]);

