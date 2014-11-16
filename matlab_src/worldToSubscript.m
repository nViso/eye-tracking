function [m n] = worldToSubscript(xlim,ylim,x,y)
m = floor(y - ylim(1) +1);
n = floor(x - xlim(1) +1);