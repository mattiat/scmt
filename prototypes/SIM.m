% SIM Single Input Module 

clear all;

% constants
TIME_STEP = 0.01;
tmax = 10;
timeArray = 0:TIME_STEP:tmax;
n = length(timeArray);

% network
y1Ind=1; y2Ind=2; y3Ind=3; xInd=4;
productionRate = [10,10,10,60];
degradationRate = [1,1,1,1.2];
steadyState(y1Ind) = productionRate(y1Ind)/degradationRate(y1Ind);
steadyState(y2Ind) = productionRate(y2Ind)/degradationRate(y2Ind);
steadyState(y3Ind) = productionRate(y3Ind)/degradationRate(y3Ind);
steadyState(xInd) = productionRate(xInd)/degradationRate(xInd);
threshold(y1Ind) = 7;
threshold(y2Ind) = 14;
threshold(y3Ind) = 21;
sx(1:floor(n/3)) = 1; % signal that activates x until time n/2
sx(floor(n/3):floor(2/3*n)) = 0;
sx(floor(2/3*n):n) = 1;


% preconditions
x0 = 0;
y0 = zeros(1,3);

% initializing
x = zeros(1,n);
y = [zeros(1,n);zeros(1,n);zeros(1,n)];
ex_y = [zeros(1,n);zeros(1,n);zeros(1,n)];
ex_x = zeros(1,n);

x(1) = x0;
y(y1Ind,1) = y0(y1Ind);
y(y2Ind,1) = y0(y2Ind);
y(y3Ind,1) = y0(y3Ind);
ex_y(y1Ind,1) = y0(y1Ind);
ex_y(y2Ind,1) = y0(y2Ind);
ex_y(y3Ind,1) = y0(y3Ind);

tau = zeros(1,4);
peak = zeros(1,4);
base = zeros(1,4);
active = [false,false,false,true];

% debugging
level_y = [zeros(1,n);zeros(1,n);zeros(1,n)];

for tInd = 2:n,
    % approximation
    % x
    if sx(tInd)<0.5,
        dx = -degradationRate(xInd)*x(tInd-1);
    else
        dx = productionRate(xInd)-degradationRate(xInd)*x(tInd-1);
    end;
    x(tInd) = x(tInd-1) + dx*TIME_STEP;
    % y
    for yInd = 1:y3Ind,
        level_y(yInd,tInd) = hillFunction(x(tInd-1),threshold(yInd),100);
        dy = (level_y(yInd,tInd)*productionRate(yInd))-degradationRate(yInd)*y(yInd,tInd-1);
        y(yInd,tInd) = y(yInd,tInd-1) + dy*TIME_STEP;
    end;
    
    % simulation
    % x
    if sx(tInd)<0.5 && active(xInd) == true,
        active(xInd)=~active(xInd);
        tau(xInd) = timeArray(tInd);
    end;
    if sx(tInd)>0.5 && active(xInd) == false,
        active(xInd)=~active(xInd);
        tau(xInd) = timeArray(tInd);
    end;
    localTime = timeArray(tInd) - tau(xInd);
    if active(xInd)
        ex_x(tInd) = steadyState(xInd)*(1-exp(-degradationRate(xInd).*localTime));
    else
        ex_x(tInd) = steadyState(xInd)*(exp(-degradationRate(xInd).*localTime));
    end;
    % y
    for yInd = 1:y3Ind,
        level_y(yInd,tInd) = hillFunction(ex_x(tInd-1),threshold(yInd),100);
        if level_y(yInd,tInd)>0.5 && active(yInd)==false, % became active
            tau(yInd) = timeArray(tInd);
            active(yInd)=~active(yInd);
            base(yInd) = ex_y(yInd,tInd-1);
        end;
        if level_y(yInd,tInd)<0.5 && active(yInd)==true, % became inactive
            tau(yInd) = timeArray(tInd);
            active(yInd)=~active(yInd);
            peak(yInd) = ex_y(yInd,tInd-1);
        end;
        localTime = timeArray(tInd) - tau(yInd);
        if active(yInd)==true % producing
            ex_y(yInd,tInd) = steadyState(yInd)-(steadyState(yInd)-base(yInd))*(exp(-degradationRate(yInd)*localTime));
        end;
        if active(yInd)==false % degrading
            ex_y(yInd,tInd) = peak(yInd)*(exp(-degradationRate(yInd)*localTime));
        end;
    end;
end;

subplot(411); plot(timeArray,[ex_x; x]);
subplot(412); plot(timeArray,[ex_y(y1Ind,:); y(y1Ind,:)]); 
subplot(413); plot(timeArray,[ex_y(y2Ind,:); y(y2Ind,:)]); 
subplot(414); plot(timeArray,[ex_y(y3Ind,:); y(y3Ind,:)]); 