function [ level ] = activityLevel( concentration, threshold )
n = 100;
level = (concentration)^n / ((concentration)^n + (threshold)^n);
%if concentration > threshold
%    level = 1;
%else
%    level = 0;
%end;
end

