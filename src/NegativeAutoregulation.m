% 0NE GENE NEGATIVE AUTOREGULATION

clear all;

% constants
TIME_STEP = 0.01;
tmax = 5;
timeArray = 0:TIME_STEP:tmax;
n = length(timeArray);

% network
productionRate_x = 25;
degradationRate_x = 2;
steadyState_x = productionRate_x/degradationRate_x;
threshold_x = 10;

% preconditions
x0 = 0;

% initializing
x = zeros(1,n);
x(1) = x0;
ex_x = zeros(1,n);
tau_x = 0;
active_x = false;

for tInd = 2:n,
    % approximation
    level_x = activityLevel(x(tInd-1),threshold_x);
    simple = productionRate_x;
    autoreg = level_x*(-productionRate_x);
    dx = (simple+autoreg) - degradationRate_x*x(tInd-1);
    x(tInd) = x(tInd-1) + dx*TIME_STEP;
    
    % simulation
    if level_x >0.01 && active_x==false,
        tau_x = timeArray(tInd-1);
        active_x = true;
    end;
    localTime = timeArray(tInd) - tau_x;
    autoreg = level_x*(-steadyState_x)*(1-exp(-degradationRate_x.*localTime));
    simple = steadyState_x*(1-exp(-degradationRate_x.*timeArray(tInd)));
    ex_x(tInd) = simple + autoreg;
end;

plot(timeArray,[ex_x; x]);