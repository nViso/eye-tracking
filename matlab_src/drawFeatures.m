clc;

close all;

clearvars -except testsData ;

selectedtests = 1:length(testsData);
selectedGraphs = 1: size(testsData{1},2);
selectedPeople = 1: size(testsData{1},1);
featureBank = fieldnames(testsData{1}{1,1});
selectedFeature = featureBank([5 6 13 14]);


%%
cols = length(selectedPeople);
rows = length(selectedFeature);

for u = selectedtests
    for i = selectedGraphs
        figure;
        [sx sy sw sh] = getMatlabWindowPosition;
        set(gcf,'Position',[sx sy sw sh]);
        set(gcf,'NumberTitle','off','Name',[testsData{u}{1,1}.testname ' ' testsData{u}{1,i}.tracename])
        for j = selectedPeople
            for k = 1:length(selectedFeature)
                col = find(selectedPeople==j);
                row = k;
                subplotAxis = optimalSubplot(gcf,cols,rows,col,row);
                data = eval(['testsData{u}{j,i}.' selectedFeature{k}]);
                
                if size(data,2) >=20
                    imagesc(data);
                end
                
                if size(data,2) ==2
                    data(:,3) = [1:length(data)]';
                    plot3(data(:,1),data(:,2),data(:,3),'-');
                    hold on;
                    scatter3(data(:,1),data(:,2),data(:,3),20,1:size(data,1),'filled');
                    hold off;
                end
                
                if size(data,2) == 1
                    plot(data,'.-');
                end
                
                if k == 1
                    title([testsData{u}{col,1}.testname ' ' testsData{u}{col,1}.username ' ' selectedFeature{k}],'Interpreter','non');
                else
                    title([selectedFeature(k)]);
                end
            end
            
        end
        
        %     save2pdf([num2str(i) '.pdf'],gcf);
    end
    
end

%%
clearvars -except testsData ; featureBank;

