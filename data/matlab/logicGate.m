function [ level ] = logicGate( concentration, threshold )
if concentration > threshold
   level = 1;
else
   level = 0;
end;
end