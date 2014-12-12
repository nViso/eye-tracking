close all; clc;clear;
addpath('../');
addpath('../IMUProcessing');
baseDir = '/Users/ZhipingJiang/trackingdata/video_parsing/';
movFiles = dir([baseDir '*.mov']);
movFiles = movFiles([ 24]);
%%

datas = {};
for i = 1:length(movFiles)
    fileName = [baseDir movFiles(i).name];
    disp(fileName);
    fileNameHeader = strrep(fileName,'vision.mov','');
    datas{i} = ParseFusedData(fileNameHeader);
end

clearvars -except datas baseDir movFiles;

%% best face ever.
symmetryCut = 0.1;
symmetryScores  = [];
for i = 1:length(datas)
   symmetryScores = [symmetryScores ; datas{i}.vision.symmetryScore]; 
   
end
[f, x] = ecdf(symmetryScores);
cutOffRatio = x(find(f < symmetryCut,1,'last'));

findSymmetryReadyIndexFunc = @(x) find(x.vision.symmetryScore<cutOffRatio);
[indeces] = cellfun(findSymmetryReadyIndexFunc,datas,'UniformOutput',false);

minValues = {};
for i = 1:length(indeces)
    [values,ind] = max( datas{i}.vision.heronAreaComperhensive(indeces{i}).* (1./datas{i}.vision.symmetryScore(indeces{i})) );
    indeces{i} = indeces{i}(ind);
    minValues{i} = values;
    
    if isempty(values)
        minValues{i} = nan;
    end
end
xbestValue = nanmin(cell2mat(minValues));
xbestVideoIndex = find(cell2mat(minValues) ==xbestValue);
xbestFrameIndex = indeces{xbestVideoIndex};
xbestFrameIndex = 360;

xpoints = reshape(datas{xbestVideoIndex}.vision.allFeaturePoints(xbestFrameIndex,:)',[2 7])';
noseRatio = norm((xpoints(5,:)+xpoints(6,:))/2 - (xpoints(1,:)+xpoints(2,:))/2 )/ norm((xpoints(5,:)+xpoints(6,:))/2 - xpoints(7,:) )

markerInserter = vision.MarkerInserter('FillColor','White','Fill',true,'Size',10,'Opacity',1);
videoReader = VideoReader(datas{xbestVideoIndex}.vision.videoFileName);
xbestImage = read(videoReader,xbestFrameIndex);
xmarked = step(markerInserter,xbestImage,int32(xpoints));
figure;imshow(xmarked);


%% fine-grained tuning.
symmetryCut = 0.05;
symmetryScores  = [];
for i = 1:length(datas)
   symmetryScores = [symmetryScores ; datas{i}.vision.symmetryScore]; 
    
end
[f, x] = ecdf(symmetryScores);
cutOffRatio = x(find(f < symmetryCut,1,'last'));

findSymmetryReadyIndexFunc = @(x) find(x.vision.symmetryScore<cutOffRatio);
[indeces] = cellfun(findSymmetryReadyIndexFunc,datas,'UniformOutput',false);

h = [];
for i = 1:length(indeces)
    points = datas{i}.vision.allFeaturePoints(indeces{i},:);
    points = reshape(points',[2 7 size(points,1)]);
    points(:,[5 6 7],:) = [];
    for j = 1:size(points,3)
        distM = pdist2(points(:,:,j)',points(:,:,j)');
        distM = distM / distM(3,4);
        h = cat(3,h,distM);
    end
end
medianh = median(h,3);
eyehorizon = mdscale(medianh,2);
baseShape = [0.4 0 ; -0.4 0 ; 1 0; -1 0]/2;
[d,z,t] = procrustes(baseShape,eyehorizon);
eyehorizon = t.b*eyehorizon*t.T +t.c;

h = [];
for i = 1:length(indeces)
    points = datas{i}.vision.allFeaturePoints(indeces{i},:);
    points = reshape(points',[2 7 size(points,1)]);
    for j = 1:size(points,3)
        distM = pdist2(points(:,:,j)',points(:,:,j)');
        distM = distM / distM(3,4);
        h = cat(3,h,distM);
    end
end
medianh = median(h,3);
allscaled = mdscale(medianh,2);
noseRatio = norm((allscaled(5,:)+allscaled(6,:))/2 - (allscaled(1,:)+allscaled(2,:))/2 )/ norm((allscaled(5,:)+allscaled(6,:))/2 - allscaled(7,:) )


[d,z,t] = procrustes(eyehorizon,allscaled(1:4,:));
t.c = [t.c ; t.c(1:3,:)];
allscaled = t.b*allscaled*t.T +t.c;

[d,z,t] = procrustes(eyehorizon,xpoints(1:4,:));
t.c = [t.c ; t.c(1:3,:)];
xpoints = t.b*xpoints*t.T +t.c;

xpoints([5 6],1) =xpoints([5 6],1)- (xpoints(5,1)+xpoints(6,1))/2;

%%
dlmwrite('facial2dmodel.txt',xpoints,'delimiter',' ');