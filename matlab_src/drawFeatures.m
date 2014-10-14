clc;

close all;

clearvars -except testsData ;

selectedtests = 1:4;
selectedGraphs = 1: size(testsData{1},2);

peopleNames = {};
for i = 1:length(testsData)
    for j = 1:size(testsData{i},1)
        peopleNames = [ peopleNames ; testsData{i}{j}.username ];
    end
end
peopleNames = unique(peopleNames);
selectedPeople = peopleNames;

featureBank = fieldnames(testsData{1}{1,1});
selectedFeature = featureBank([4 5 6 13 14 17 18 19 20]);
%%
cols = length(selectedPeople);
rows = length(selectedFeature);

for u = selectedtests
    for i = selectedGraphs
        figure;
        set(gcf,'visible','off');
        [sx sy sw sh] = getMatlabWindowPosition;
        set(gcf,'Position',[sx sy 1000 1950]);
        
        set(gcf,'NumberTitle','off','Name',[testsData{u}{1,1}.testname ' ' testsData{u}{1,i}.tracename])
        for j = 1:length(selectedPeople)
            for k = 1:length(selectedFeature)
                col = j;
                userIndex = find(cellfun(@(x) strcmp(x.username,selectedPeople(j)), testsData{u}(:,i),'UniformOutput',true));
                row = k;
                subplotAxis = optimalSubplot(gcf,cols,rows,col,row);
                
                data = eval(['testsData{u}{userIndex,i}.' selectedFeature{k}]);
                
                if size(data,2) >=20
                    imagesc(data);
                end
                
                if size(data,2) ==2
%                     data(:,3) = [1:length(data)]';
%                     plot3(data(:,1),data(:,2),data(:,3),'-');
                    
                    plot(data(:,1),data(:,2),'-');
                    hold on;
%                     scatter3(data(:,1),data(:,2),data(:,3),20,1:size(data,1),'filled');
                    scatter(data(:,1),data(:,2),20,1:size(data,1),'filled');
                    hold off;
                    set(gca,'YDir','reverse');
                end
                
                if size(data,2) == 1
                    plot(data,'.-');
                end
                
                if k == 1
                    title([testsData{u}{col,1}.username ' ' selectedFeature{k}],'Interpreter','non');
                else
                    title([selectedFeature(k)]);
                end
            end
            
        end
        
        save2pdf([testsData{u}{1,1}.testname ' ' testsData{u}{1,i}.tracename '.pdf'],gcf);
    end
    
end

%%
clearvars -except testsData  featureBank;

