addpath ('./dtw');
close all;

selectedtests = 1:length(testsData);
selectedGraphs = 1: size(testsData{1},2);
selectedPeople = 1: size(testsData{1},1);
featureBank = fieldnames(testsData{1}{1,1});
selectedFeatures = featureBank([14]);

%%
cols = length(selectedPeople);
rows = length(selectedtests);

for i = selectedGraphs
    for j = 1:length(selectedFeatures)
        for k = selectedPeople
            figure;
            [sx sy sw sh] = getMatlabWindowPosition;
            set(gcf,'Position',[sx sy sw sh]);
            set(gcf,'NumberTitle','off','Name',[testsData{1}{k,i}.username ' ' testsData{1}{k,i}.tracename ' ' selectedFeatures{j}]);
            baseM =eval([ 'testsData{1}{k,i}.' selectedFeatures{j}]);
            baseM = pdist2(baseM,baseM);
            for l = selectedPeople
                for m = selectedtests
                    col = l;
                    row = m;
                    testM = eval([ 'testsData{m}{l,i}.' selectedFeatures{j}]);
                    testM = pdist2(testM,testM);
                    SM = simmx(baseM,testM);
%                     [p,q,C] = dp(1-SM);
                    subplotAxis = optimalSubplot(gcf,cols,rows,col,row);
%                     hold on;
                    imagesc(SM);
%                     plot(q,p,'r'); 
%                     hold off;
                end
            end
        end
    end
end