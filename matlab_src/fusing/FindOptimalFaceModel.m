close all; clc;clear;
baseDir = '/Users/ZhipingJiang/trackingdata/video_parsing/';
movFiles = dir([baseDir '141105_1449vision.mov']);

datas = {};
for i = 1:length(movFiles)
    fileName = [baseDir movFiles(i).name];
    disp(fileName);
    fileNameHeader = strrep(fileName,'vision.mov','');
    datas{i} = ParseFusedData(fileNameHeader);
end

clearvars -except datas baseDir movFiles;

%% 
h = [];
for i = 1:length(datas)
    data = datas{i}.vision.allFeaturePoints;
    d2 = reshape(data',[2 7 size(data,1)]);
    for j = 1:size(d2,3) 
        p = pdist2(d2(:,:,j)',d2(:,:,j)');
        distancesum = sum(sum(triu(p)));
        p = p / distancesum;
        h = cat(3,h,p);
    end
end

hmedian = median(h,3);
p = mdscale(hmedian,2);

%%


base = datas{1}.vision.allFeaturePoints;
base = reshape(base',2,7,size(base,1));
X = base(:,:,1)';
for i = 1:size(base,3)
   [~, Z] = procrustes(X,base(:,:,i)');
   base(:,:,i) = Z';
end
base = reshape(base,14,size(base,3))';


samplingGap = 1;
data = base(1:samplingGap:end,:);
data = unique(data,'rows');
distM = pdist2(data,data);

p = mdscale(distM,3);


mbase = median(base);
p2 = reshape(mbase,2,7)';

display('done');
