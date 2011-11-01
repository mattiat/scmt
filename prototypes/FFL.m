% FEED FORWARD LOOP

clear all;

% constants
TIME_STEP = 0.01;
tmax = 2;
timeArray = 0:TIME_STEP:tmax;
n = length(timeArray);

% network
coherent = false;
xInd=1; yInd=2; zInd=3;
productionRate = [30,17,25];
degradationRate = [2,1,5];
steadyState(xInd) = productionRate(xInd)/degradationRate(xInd);
steadyState(yInd) = productionRate(yInd)/degradationRate(yInd);
steadyState(zInd) = productionRate(zInd)/degradationRate(zInd);
threshold_xy = 7;
threshold_yz = 8;
threshold_xz = 10;

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

tau_y = 0;
tau_xz = 0;
tau_yz = 0;
active_y = false;
active_xz = false;
active_yz = false;

% simulation x
ex_x = steadyState(xInd)*(1-exp(-degradationRate(xInd)*timeArray));

for tInd = 2:n,
    % approximation
    % x
    dx = productionRate(xInd)-degradationRate(xInd)*x(tInd-1);
    x(tInd) = x(tInd-1) + dx*TIME_STEP;
    % y
    level_y = hillFunction(x(tInd-1),threshold_xy,100);
    dy = (level_y*productionRate(yInd))-degradationRate(yInd)*y(tInd-1);
    y(tInd) = y(tInd-1) + dy*TIME_STEP;
    % z
    level_xz = hillFunction(x(tInd-1),threshold_xz,100);
    level_yz = hillFunction(y(tInd-1),threshold_yz,100);
    dz = 0;
    if level_xz>0.5 && level_yz<0.5 % above x threshold?
        dz_x = 0.5*level_xz*productionRate(zInd)-degradationRate(zInd)*z(tInd-1);
        dz = dz_x;
    end;
    if level_xz<0.5 && level_yz>0.5 % above y threshold?
        if coherent == true,
            dz_y = 0.5*level_yz*productionRate(zInd)-degradationRate(zInd)*z(tInd-1);
        else
            dz_y = 0.5*level_yz*(-productionRate(zInd))-degradationRate(zInd)*z(tInd-1);
        end;
        dz = dz_y;
    end;
    if level_yz>0.5 && level_yz>0.5 % both x and y threshold?
        productionRate_xz = 0.5*level_xz*productionRate(zInd);
        if coherent == true,
            productionRate_yz = 0.5*level_yz*productionRate(zInd);
        else
            productionRate_yz = 0.5*level_yz*(-productionRate(zInd));
        end;
        dz = productionRate_xz + productionRate_yz -degradationRate(zInd)*z(tInd-1);
    end;
    z(tInd) = z(tInd-1) + dz*TIME_STEP;
    
    % simulation
    % y
    level_y = hillFunction(ex_x(tInd-1),threshold_xy,100);
    if level_y>0.5 && active_y==false,
        active_y = true;
        tau_y = timeArray(tInd-1);
    end;
    if level_y>0.5
        localTime_y = timeArray(tInd) - tau_y;
        ex_y(tInd) = level_y*steadyState(yInd)*(1-exp(-degradationRate(yInd)*localTime_y));
    end;
    % z
    level_xz = hillFunction(ex_x(tInd-1),threshold_xz,100);
    level_yz = hillFunction(ex_y(tInd-1),threshold_yz,100);
    if level_xz>0.5 && active_xz==false, % above x threshold?
        active_xz=true;
        tau_xz = timeArray(tInd-1);
    end;
    if level_yz>0.5 && active_yz==false, % above y threshold?
        active_yz=true;
        tau_yz = timeArray(tInd-1);
    end;
    if level_xz>0.5 && level_yz<0.5
        localTime_xz = timeArray(tInd) - tau_xz;
        ex_z(tInd) = 0.5*level_xz*steadyState(zInd)*(1-exp(-degradationRate(zInd)*localTime_xz));
    end;
    if level_xz<0.5 && level_yz>0.5
        localTime_yz = timeArray(tInd) - tau_yz;
        ex_z(tInd) = 0.5*level_yz*steadyState(zInd)*(1-exp(-degradationRate(zInd)*localTime_yz));
    end; 
    if level_yz>0.5 && level_yz>0.5 % both x and y threshold?
        localTime_xz = timeArray(tInd) - tau_xz;
        localTime_yz = timeArray(tInd) - tau_yz;
        productionRate_xz = 0.5*level_xz*steadyState(zInd);
        if coherent == true,
            productionRate_yz = 0.5*level_yz*steadyState(zInd);
        else
            productionRate_yz = 0.5*level_yz*(-steadyState(zInd));
        end;
        xProd = productionRate_xz*(1-exp(-degradationRate(zInd)*localTime_xz));
        yProd = productionRate_yz*(1-exp(-degradationRate(zInd)*localTime_yz));
        ex_z(tInd) = xProd + yProd;
    end;    
end;

subplot(311); plot(timeArray,[ex_x; x]);
subplot(312); plot(timeArray,[ex_y; y]);
subplot(313); plot(timeArray,[ex_z; z]);

