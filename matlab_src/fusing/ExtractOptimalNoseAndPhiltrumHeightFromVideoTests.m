%% preparation.
clc;clear;
addpath('../');
addpath('../IMUProcessing');
baseDir = '/Users/ZhipingJiang/trackingdata/video_parsing/';
testFiles = dir([baseDir '*.test']);
noseHeights = [];
philtrumHeights = [];
fileCount = 0;
for i = 1:length(testFiles)
    paramFiles = dir([baseDir testFiles(i).name '_*']);
    baseFileName = testFiles(i).name;
    for j = 1:length(paramFiles)
        fileName = paramFiles(j).name;
        visionFilePath = [baseDir fileName];
        heightPart = strrep(fileName,[ baseFileName '_'],'');
        heights = strsplit(heightPart,'_');
        noseHeights = unique([noseHeights; str2num(heights{1})]);
        philtrumHeights = unique([philtrumHeights; str2num(heights{2})]);
        fileCount = fileCount+1;
    end
end
noseHeights = sort(noseHeights);
philtrumHeights = sort(philtrumHeights);
datas = cell(length(noseHeights),length(philtrumHeights),length(testFiles));
fileNames = cell(length(noseHeights),length(philtrumHeights),length(testFiles));
%%
for i = 1:length(testFiles)
    paramFiles = dir([baseDir testFiles(i).name '_*']);
    baseFileName = testFiles(i).name;
    for j = 1:length(paramFiles)
        fileName = paramFiles(j).name;
        visionFilePath = [baseDir fileName];
        heightPart = strrep(fileName,[baseFileName '_'],'');
        heights = strsplit(heightPart,'_');
        noseHeight = str2num(heights{1});
        philtrumHeight = str2num(heights{2});
        fileNames{noseHeights ==noseHeight ,philtrumHeights == philtrumHeight,i} = visionFilePath;
    end
end
%% read data.
progress = zeros(size(fileNames));
for i = 1:length(testFiles)
    if isempty(find(cellfun(@(x) isempty(x), fileNames(:,:,i)) == 0, 1)) == 1
        continue;
    end
    paramFiles = dir([baseDir testFiles(i).name '_*']);
    baseFileName = testFiles(i).name;
    sensorFileName = strrep(baseFileName,'vision.test','sensor.txt');
    sensorFilePath = [baseDir sensorFileName];
    sensorData = sensorTextReadFromDisk(sensorFilePath);
    sensorData = DeviceMotionAxesShift(sensorData);
    tic;
    for j = 1:size(fileNames,1) 
       if isempty(find(cellfun(@(x) isempty(x), fileNames(j,:,i)) == 0, 1)) == 1
           continue;
       end
%        tic;
       parfor k = 1:size(fileNames,2)
           display(fileNames{j,k,i});
           if isempty(fileNames{j,k,i}) 
               continue;
           end
           heightPart = strrep(fileNames{j,k,i},[baseDir baseFileName '_'],'');
           heights = strsplit((heightPart),'_');
           noseHeight = str2num(heights{1});
           philtrumHeight = str2num(heights{2});
           
           data = ParseFusedDataWithSensorData(sensorData,fileNames{j,k,i});
           fields = {'sensor','quatFaceToBN','quatFaceToNN','quatFaceToNNIdeal','quatSensorBNToNN','quatSensorVisionGap','phoneAngleNN'};
           data = rmfield(data,fields);
           data.noseHeight = noseHeight;
           data.philtrumHeight = philtrumHeight;
           data.fileName = baseFileName;
           datas{j,k,i} = data;
           progress(j,k,i) = 1;
           
%            display(['progress:(' num2str(sum(progress(:))) ',' num2str(fileCount) ')']);
       end
%        toc;
    end
    toc;
end

%%
status = cellfun(@(x) ~isempty(x), datas);
datas(:,:,squeeze(sum(sum(status,1),2)) == 0) = [];
status(:,:,squeeze(sum(sum(status,1),2)) == 0) = [];
datas(:,sum(status)==0) = [];
status = cellfun(@(x) ~isempty(x), datas);
[~, closestIdx] = bwdist(status);
datas(:) = datas(closestIdx);

% noseRMSFunc = @(x) median((rms((x.vision.allFeaturePoints(:,9:end) - x.vision.reprojectedFeaturePoints(:,9:end))')));
% faceAngleDiffFunc = @(x) sum(std(abs(diff(x.faceAngleBN))));
% faceAngleStdFunc = @(x) sum(std(((x.faceAngleBN))));
allRMSMedianFunc = @(x) median(((x.reprojectionRMS)));
allRMSStdFunc = @(x) abs(mad(x.reprojectionRMS,0));
% peak2rmcFunc = @(x) peak2rms(x.reprojectionRMS);
% distanceFunc = @(x) median(rssq(x.vision.tvec,2));
% rssqAllRMSFunc = @(x) rssq(x.reprojectionRMS);
% noseRatioFunc = @(x) median(x.vision.noseRatio);
% noseAngleFunc = @(x) median(x.vision.noseAngle);
% faceNNAngle1Func = @(x) median(x.faceAngleNN(:,1));
% faceNNAngle2Func = @(x) median(x.faceAngleNN(:,2));
% faceNNAngle3Func = @(x) median(x.faceAngleNN(:,3));
% maxAreaFunc = @(x) max(x.vision.heronFaceArea);
% maxAreaSmallerFunc = @(x) max(x.vision.heronNoseArea);

% xfaceNNDiffStd = squeeze(cellfun(faceAngleDiffFunc,datas));
% xfaceNNStd = squeeze(cellfun(faceAngleStdFunc,datas));
xreprojectionRMS = squeeze(cellfun(allRMSMedianFunc,datas));
xreprojectionRMSstd = squeeze(cellfun(allRMSStdFunc,datas));
% xnoseRMS = squeeze(cellfun(noseRMSFunc,datas));
% xdistance = squeeze(cellfun(distanceFunc,datas));
% xheronArea = squeeze(cellfun(maxAreaFunc,datas));
% xheronAreaSmall = squeeze(cellfun(maxAreaSmallerFunc,datas));


% xfaceNNangle1 = squeeze(cellfun(faceNNAngle1Func,datas));
% xfaceNNangle2 = squeeze(cellfun(faceNNAngle2Func,datas));
% xfaceNNangle3 = squeeze(cellfun(faceNNAngle3Func,datas));

xreprojectionRMS = imresize(xreprojectionRMS,3);
xreprojectionRMSstd = imresize(xreprojectionRMSstd,3);
xreprojectionRMSstd = abs(xreprojectionRMSstd);
xcombine = xreprojectionRMS.*xreprojectionRMSstd;
hgaussian = fspecial('gaussian',7,7);
xcombineG = imfilter(xcombine,hgaussian,'replicate');
noseHeights = imresize(noseHeights,3);
philtrumHeights = imresize(philtrumHeights,3);

[r c] = find(xcombineG == min(min(xcombineG)));

[noseHeights(r,1) philtrumHeights(c,1)]