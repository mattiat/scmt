function [ level ] = hillFunction( concentration, threshold, n )
level = (concentration)^n / ((concentration)^n + (threshold)^n);
end