clc;clear;close all;
matFiles = dir('./Best2DFace.*.mat');

for i = 1:length(matFiles)
    nameString = matFiles(i).name;
    load(nameString);
    nameString = strrep(nameString,'.mat','.jpg');
    xmarked = imresize(xmarked,0.5);
    imwrite(xmarked,nameString);
end