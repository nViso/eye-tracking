close all; clc;clear;
addpath('../');
addpath('../IMUProcessing');
baseDir = '/Users/ZhipingJiang/trackingdata/video_parsing/';
movFiles = dir([baseDir '*.mov']);

datas = {};
for i = 1:length(movFiles)
    fileName = [baseDir movFiles(i).name];
    disp(fileName);
    fileNameHeader = strrep(fileName,'vision.mov','');
    datas{i} = ParseFusedData(fileNameHeader);
end

clearvars -except datas baseDir movFiles;
%% estimate error model using linear regression.
headMobilities = cellfun(@(x) x.headMobility,datas)';
headImmobileIndex = find(headMobilities <25 & headMobilities >0);

faceAngleC = [];
gapAngle = [];
reprojectionRMS = [];
for i  = 1:length(headImmobileIndex)
    
    gapAngle = ([gapAngle; datas{headImmobileIndex(i)}.gapAngle]);
    faceAngleC = ([faceAngleC ; datas{headImmobileIndex(i)}.faceAngleC]);
    reprojectionRMS = [reprojectionRMS datas{headImmobileIndex(i)}.reprojectionRMS];
end

gapAngle = unwrap(deg2rad(gapAngle));
% gaplp = abs(sum([filtfilt(ones(1,10)/10,1,gapAngle)]'));
% removeIndex = find(gaplp >  2* std(gaplp));
% gapAngle(removeIndex,:) = [];
faceAngleC = unwrap(deg2rad(faceAngleC));
% faceAngleC(removeIndex,:) = [];
g1 = gapAngle(:,1);
g2 = gapAngle(:,2);
g3 = gapAngle(:,3);

gap1Model = fitlm(faceAngleC,g1,'RobustOpts','on');
gap2Model = fitlm(faceAngleC,g2,'RobustOpts','on');
gap3Model = fitlm(faceAngleC,g3,'RobustOpts','on');
%% correct the error.

datasCorrected = {};
for i = 1:length(movFiles)
    fileName = [baseDir movFiles(i).name];
    disp(fileName);
    fileNameHeader = strrep(fileName,'vision.mov','');
    datasCorrected{i} = ParseFusedData(fileNameHeader,gap1Model,gap2Model,gap3Model);
end
%%
clearvars -except datas datasCorrected;
gapAngle = [];
gapAngle2 = [];
for i  = 1:length(datasCorrected)
    gapAngle  = ([gapAngle; datas{i}.gapAngle]);
    gapAngle2 = ([gapAngle2; datasCorrected{i}.gapAngle]);
end
gapAngle = (deg2rad(gapAngle));
gapAngle2 = (deg2rad(gapAngle2));

mean(gapAngle)
mean(gapAngle2)
std(gapAngle)
std(gapAngle2)
