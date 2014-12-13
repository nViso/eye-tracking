clc;clear;close all;
baseDir = '/Users/ZhipingJiang/trackingdata/video_parsing/141105_1446vision/';

leftEyeFiles = dir([baseDir 'L*.jpg']);
RightEyeFiles = dir([baseDir 'R*.jpg']);
leftEyeFiles = arrayfun(@(x) x.name, leftEyeFiles,'UniformOutput',false);
RightEyeFiles= arrayfun(@(x) x.name, RightEyeFiles,'UniformOutput',false);
leftEyeFiles = sort_nat(leftEyeFiles);
RightEyeFiles = sort_nat(RightEyeFiles);

left = {};
right = {};
for ind = 1:length(leftEyeFiles)
    filename = [baseDir leftEyeFiles{ind}];
    left{ind} = imread(filename);
    filename = [baseDir RightEyeFiles{ind}];
    right{ind} = imread(filename);
end

clearvars -except left right;


%%
close all;
for imageIndex = 20
    for angles = 7
        img = left{imageIndex};
        tic;
        [r c] = LocatePupilCenter(img,angles);
        toc;
        figure;imshow(img);
        hold on;
        plot(c,r,'o');
        hold off;
    end
end
