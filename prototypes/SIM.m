% SIM Single Input Module 

clear all;

% constants
TIME_STEP = 0.01;
tmax = 3.5;
timeArray = 0:TIME_STEP:tmax;
n = length(timeArray);

% network
y1Ind=1; y2Ind=2; y3Ind=3; xInd=4;
productionRate = [10,10,10,60];
degradationRate = [1,1,1,2];
steadyState(y1Ind) = productionRate(y1Ind)/degradationRate(y1Ind);
steadyState(y2Ind) = productionRate(y2Ind)/degradationRate(y2Ind);
steadyState(y3Ind) = productionRate(y3Ind)/degradationRate(y3Ind);
steadyState(xInd) = productionRate(xInd)/degradationRate(xInd);
threshold(y1Ind) = 7;
threshold(y2Ind) = 14;
threshold(y3Ind) = 21;

% preconditions
x0 = 0;
y0 = zeros(1,3);

% initializing
x = zeros(1,n);
y = [zeros(1,n);zeros(1,n);zeros(1,n)];
ex_y = [zeros(1,n);zeros(1,n);zeros(1,n)];

x(1) = x0;
y(y1Ind,1) = y0(y1Ind);
y(y2Ind,1) = y0(y2Ind);
y(y3Ind,1) = y0(y3Ind);
ex_y(y1Ind,1) = y0(y1Ind);
ex_y(y2Ind,1) = y0(y2Ind);
ex_y(y3Ind,1) = y0(y3Ind);

tau_y = zeros(1,3);
active_y = [false,false,false];

% debugging
level_y = [zeros(1,n);zeros(1,n);zeros(1,n)];

ex_x = steadyState(xInd)*(1-exp(-degradationRate(xInd).*timeArray));

for tInd = 2:n,
    % approximation
    % x
    dx = productionRate(xInd)-degradationRate(xInd)*x(tInd-1);
    x(tInd) = x(tInd-1) + dx*TIME_STEP;
    % y
    for yInd = 1:y3Ind,
        level_y(yInd,tInd) = hillFunction(x(tInd-1),threshold(yInd),100);
        dy = (level_y(yInd,tInd)*productionRate(yInd))-degradationRate(yInd)*y(yInd,tInd-1);
        y(yInd,tInd) = y(yInd,tInd-1) + dy*TIME_STEP;
    end;
    
    % simulation
    for yInd = 1:y3Ind,
        level_y(yInd,tInd) = hillFunction(ex_x(tInd-1),threshold(yInd),100);
        if level_y(yInd,tInd)>0.5 && active_y(yInd)==false, % reset tau?
            tau_y(yInd) = timeArray(tInd);
            active_y(yInd)=true;
        end;
        if level_y(yInd,tInd)>0.5 % producing?
            localTime = timeArray(tInd) - tau_y(yInd);
            ex_y(yInd,tInd) = level_y(yInd,tInd)*steadyState(yInd)*(1-exp(-degradationRate(yInd)*localTime));
        end;
    end;
end;

subplot(411); plot(timeArray,[ex_x; x]);
subplot(412); plot(timeArray,[ex_y(y1Ind,:); y(y1Ind,:)]); 
subplot(413); plot(timeArray,[ex_y(y2Ind,:); y(y2Ind,:)]); 
subplot(414); plot(timeArray,[ex_y(y3Ind,:); y(y3Ind,:)]); 