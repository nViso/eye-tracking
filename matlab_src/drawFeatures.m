clc;

close all;

clearvars -except resultsData ;


selectedGraphs = 1: size(resultsData,2);
selectedPeople = 1:4; %size(eyeTrackingData,1);
featureBank = fieldnames(resultsData{1,1});
selectedFeature = featureBank([1 2 3 4 ]);


%%
cols = length(selectedPeople);
rows = length(selectedFeature);

for i = selectedGraphs
    figure;
    [sx sy sw sh] = getMatlabWindowPosition;
    set(gcf,'Position',[sx sy sw sh]);
    for j = selectedPeople
        for k = 1:length(selectedFeature)
            col = find(selectedPeople==j);
            row = k;
            subplotAxis = optimalSubplot(gcf,cols,rows,col,row);
            data = eval(['resultsData{j,i}.' selectedFeature{k}]);
            
            if size(data,2) ==2
                %plot(data(:,1),data(:,2),'.-');
                plot(data(:,1),data(:,2),'-');
                hold on;
                scatter(data(:,1),data(:,2),20,1:size(data,1),'filled');
                hold off;
            else
                plot(data,'.-');
            end
            
            if k == 1
                title([resultsData{col,1}.testername ' ' selectedFeature{k}],'Interpreter','non');
            else
                title([selectedFeature(k)]);
            end
        end
        
    end
    
%     save2pdf([num2str(i) '.pdf'],gcf);
end

%%
clearvars -except resultsData featureBank;

