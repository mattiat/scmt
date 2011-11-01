% 0NE GENE AUTOREGULATION
% desired behavior only with a hill function with steepness > 2 so that
% once the autoregulation kicks in the activity level converges

clear all;

% constants
TIME_STEP = 0.01;
tmax = 5;
timeArray = 0:TIME_STEP:tmax;
n = length(timeArray);

% network
positive = true;
productionRate_x = 25;
degradationRate_x = 2;
steadyState_x = productionRate_x/degradationRate_x;
threshold_x = 5;

% preconditions
x0 = 0;

% initializing
x = zeros(1,n);
x(1) = x0;
ex_x = zeros(1,n);
tau_x = 0;
active_x = false;

% debug
level_x = zeros(1,n);

for tInd = 2:n,
    % approximation
    
    simple = productionRate_x;
    if positive == true
        level_x(tInd) = hillFunction(x(tInd-1),threshold_x,100);
        autoreg = level_x(tInd)*productionRate_x;
    else
        level_x(tInd) = hillFunction(x(tInd-1),threshold_x,2);
        autoreg = level_x(tInd)*(-productionRate_x);
    end;
    dx = (simple+autoreg) - degradationRate_x*x(tInd-1);
    x(tInd) = x(tInd-1) + dx*TIME_STEP;
    
    % simulation
    if positive == true
        level_x(tInd) = hillFunction(ex_x(tInd-1),threshold_x,100);
    else
        level_x(tInd) = hillFunction(ex_x(tInd-1),threshold_x,2);
    end;
    if level_x(tInd)>0.7 && active_x==false,
        tau_x = timeArray(tInd-1);
        active_x = true;
    end;
    if level_x(tInd)<0.3 && active_x==true,
        tau_x = timeArray(tInd-1);
        active_x = false;
    end;
    localTime = timeArray(tInd) - tau_x;
    if positive == true
    autoreg = level_x(tInd)*steadyState_x*(1-exp(-degradationRate_x.*localTime));
    else
    autoreg = level_x(tInd)*(-steadyState_x)*(1-exp(-degradationRate_x.*localTime));
    end;
    simple = steadyState_x*(1-exp(-degradationRate_x.*timeArray(tInd)));
    ex_x(tInd) = simple+autoreg;
end;

plot(timeArray,[ex_x; x]);
