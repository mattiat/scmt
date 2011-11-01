% TWO GENES CASCADE

clear all;

% constants
TIME_STEP = 0.01;
tmax = 5;
timeArray = 0:TIME_STEP:tmax;
n = length(timeArray);

% network
productionRate_x = 30;
productionRate_y = 20;
degradationRate_x = 2;
degradationRate_y = 1;
steadyState_x = productionRate_x/degradationRate_x;
steadyState_y = productionRate_y/degradationRate_y;
threshold_y = 10;

% preconditions
x0 = 0;
y0 = 0;

% initializing
x = zeros(1,n);
y = zeros(1,n);
x(1) = x0;
y(1) = y0;

ex_y = zeros(1,n);
ex_y(1) = y0;
tau_y = 0;
active_y = false;

% debugging
level_y = zeros(1,n);

ex_x = steadyState_x*(1-exp(-degradationRate_x.*timeArray));

for tInd = 2:n,
    % approximation
    dx = productionRate_x-degradationRate_x*x(tInd-1);
    x(tInd) = x(tInd-1) + dx*TIME_STEP;
    
    level_y(tInd) = activityLevel(x(tInd-1),threshold_y);
    dy = (level_y(tInd)*productionRate_y)-degradationRate_y*y(tInd-1);
    y(tInd) = y(tInd-1) + dy*TIME_STEP;
    
    % simulation
    if level_y(tInd)>0.5 && active_y==false,
        tau_y = timeArray(tInd-1);
        active_y = true;
    end;
    if active_y == true,
        localTime = timeArray(tInd) - tau_y;
        ex_y(tInd) = level_y(tInd)*steadyState_y*(1-exp(-degradationRate_y.*localTime));
    end;
end;

subplot(211); plot(timeArray,[ex_x; x]);
subplot(212); plot(timeArray,[ex_y; y]);

