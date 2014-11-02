close all; clc;clear;
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

%% 
h = [];
for i = 1:length(datas)
    data = datas{i}.vision.allFeaturePoints;
    d2 = reshape(data',[2 6 size(data,1)]);
    for j = 1:size(d2,3) 
        p = pdist2(d2(:,:,j)',d2(:,:,j)');
        distancesum = sum(sum(triu(p)));
        p = p / distancesum;
        h = cat(3,h,p);
    end
end

hmedian = median(h,3);
p = mdscale(hmedian,3);

%%



data = [];
for i = 1:length(datas)
    data = [data; datas{i}.vision.allFeaturePoints];
end

samplingGap = 20;
data = data(1:samplingGap:end,:);
data = unique(data,'rows');
distM = pdist2(data,data);
p = mdscale(distM,2);
