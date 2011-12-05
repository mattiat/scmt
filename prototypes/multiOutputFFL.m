% multi-output Feed Forward Loop

clear all;

% constants
TIME_STEP = 0.01;
tmax = 7;
timeArray = 0:TIME_STEP:tmax;
n = length(timeArray);

% network
y1Ind=1; y2Ind=2; y3Ind=3; zInd=4; xInd=5; 
productionRate = [10,10,10,10,60];
degradationRate = [1,1,1,1.2,1.2];
steadyState(y1Ind) = productionRate(y1Ind)/degradationRate(y1Ind);
steadyState(y2Ind) = productionRate(y2Ind)/degradationRate(y2Ind);
steadyState(y3Ind) = productionRate(y3Ind)/degradationRate(y3Ind);
steadyState(zInd) = productionRate(zInd)/degradationRate(zInd);
steadyState(xInd) = productionRate(xInd)/degradationRate(xInd);
x_threshold(zInd) = 40;
x_threshold(y1Ind) = 30; % increasing x threshold
x_threshold(y2Ind) = 35;
x_threshold(y3Ind) = 40;
z_threshold(y1Ind) = 7; % decreasing y threshold
z_threshold(y2Ind) = 3;
z_threshold(y3Ind) = 1.5;
sx(1:floor(n/2)) = 1; % signal sx binds x when time < n/2
sx(floor(n/2):n) = 0;

% preconditions
x0 = 0;
z0 = 0;
y0 = zeros(1,3);

% initializing
x = zeros(1,n);
z = zeros(1,n);
y = [zeros(1,n);zeros(1,n);zeros(1,n)];
ex_x = zeros(1,n);
ex_z = zeros(1,n);
ex_y = [zeros(1,n);zeros(1,n);zeros(1,n)];

x(1) = x0;
z(1) = z0;
y(y1Ind,1) = y0(y1Ind);
y(y2Ind,1) = y0(y2Ind);
y(y3Ind,1) = y0(y3Ind);
ex_x(1) = x0;
ex_z(1) = z0;
ex_y(y1Ind,1) = y0(y1Ind);
ex_y(y2Ind,1) = y0(y2Ind);
ex_y(y3Ind,1) = y0(y3Ind);

t0 = zeros(1,4);
peak_t0 = zeros(1,4);
base_t0 = zeros(1,4);
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
    % z
    level_z = stepFunction(x(tInd),x_threshold(zInd));
    if level_z > 0
        dz = productionRate(zInd)-degradationRate(zInd)*z(tInd-1);
    else
        dz = -degradationRate(zInd)*z(tInd-1);
    end;
    z(tInd) = z(tInd-1) + dz*TIME_STEP;
    % y
    for yInd = 1:y3Ind,
        x_level = stepFunction(x(tInd),x_threshold(yInd));
        z_level = stepFunction(z(tInd),z_threshold(yInd));
        if x_level>0 || z_level>0
            dy = productionRate(yInd)-degradationRate(yInd)*y(yInd,tInd-1);
        else
            dy = -degradationRate(yInd)*y(yInd,tInd-1);
        end;
        y(yInd,tInd) = y(yInd,tInd-1) + dy*TIME_STEP;
    end;
    
%     % simulation
%     % x
%     if sx(tInd)<0.5 && active(xInd) == true,
%         active(xInd)=~active(xInd);
%         tau(xInd) = timeArray(tInd);
%     end;
%     if sx(tInd)>0.5 && active(xInd) == false,
%         active(xInd)=~active(xInd);
%         tau(xInd) = timeArray(tInd);
%     end;
%     localTime = timeArray(tInd) - tau(xInd);
%     if active(xInd)
%         ex_x(tInd) = steadyState(xInd)*(1-exp(-degradationRate(xInd).*localTime));
%     else
%         ex_x(tInd) = steadyState(xInd)*(exp(-degradationRate(xInd).*localTime));
%     end;
%     % y
%     for yInd = 1:y3Ind,
%         level_y(yInd,tInd) = hillFunction(ex_x(tInd-1),threshold(yInd),100);
%         if level_y(yInd,tInd)>0.5 && active(yInd)==false, % became active
%             tau(yInd) = timeArray(tInd);
%             active(yInd)=~active(yInd);
%             base(yInd) = ex_y(yInd,tInd-1);
%         end;
%         if level_y(yInd,tInd)<0.5 && active(yInd)==true, % became inactive
%             tau(yInd) = timeArray(tInd);
%             active(yInd)=~active(yInd);
%             peak(yInd) = ex_y(yInd,tInd-1);
%         end;
%         localTime = timeArray(tInd) - tau(yInd);
%         if active(yInd)==true % producing
%             ex_y(yInd,tInd) = steadyState(yInd)-(steadyState(yInd)-base(yInd))*(exp(-degradationRate(yInd)*localTime));
%         end;
%         if active(yInd)==false % degrading
%             ex_y(yInd,tInd) = peak(yInd)*(exp(-degradationRate(yInd)*localTime));
%         end;
%     end;
end;

subplot(511); plot(timeArray,[ex_x; x]);
subplot(512); plot(timeArray,[ex_z; z]);
subplot(513); plot(timeArray,[ex_y(y1Ind,:); y(y1Ind,:)]); 
subplot(514); plot(timeArray,[ex_y(y2Ind,:); y(y2Ind,:)]); 
subplot(515); plot(timeArray,[ex_y(y3Ind,:); y(y3Ind,:)]); 