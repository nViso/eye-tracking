clc;

close all;

clearvars -except testsData ;

%%
trainingSet = 1:3;
testSet = setdiff(1:length(testsData),trainingSet);
selectedGraphs = 1: size(testsData{1},2);

peopleNames = {};
for i = 1:length(testsData)
    for j = 1:size(testsData{i},1)
        peopleNames = [ peopleNames ; testsData{i}{j}.username ];
    end
end
peopleNames = unique(peopleNames);
selectedUsers = peopleNames;

featureBank = fieldnames(testsData{1}{1,1});
selectedFeatures = featureBank([13:14])';

%% prepare data
%positive samples
positiveSamples = cell(length(selectedGraphs),length(selectedFeatures),length(selectedUsers));
for i = 1:length(selectedGraphs)
    for j = 1:length(selectedUsers)
        for k = trainingSet
            for l = 1:length(selectedFeatures)
                % obtain the user index according to name
                userIndex = find(cellfun(@(x) strcmp(x.username,selectedUsers(j)), testsData{k}(:,i),'UniformOutput',true));
                % get his data
                data = eval(['testsData{k}{userIndex,' num2str(selectedGraphs(i)) '}.' selectedFeatures{l}]);
                positiveSamples{i,l,j} = [positiveSamples{i,l,j}; {data}];
            end
        end
    end
end
% negative samples
negativeSamples = cell(length(selectedGraphs),length(selectedFeatures),length(selectedUsers));;
for i = 1:length(selectedGraphs)
    for j = 1:length(selectedUsers)
        for k = trainingSet
            for l = 1:length(selectedFeatures)
                % obtain the user index according to name
                restUserIndex = find(cellfun(@(x) (strcmp(x.username,selectedUsers(j)) == 0), testsData{k}(:,i),'UniformOutput',true));
                
                % get his data
                for r = restUserIndex'
                    data = eval(['testsData{k}{r,' num2str(selectedGraphs(i)) '}.' selectedFeatures{l}]);
                    negativeSamples{i,l,j} = [negativeSamples{i,l,j}; {data}];
                end
                
                
            end
        end
    end
end

% test samples
testSamples = cell(length(selectedGraphs),length(selectedFeatures),length(selectedUsers));
groundTruth = cell(length(selectedGraphs),length(selectedFeatures),length(selectedUsers));
for i = 1:length(selectedGraphs)
    for j = 1:length(selectedUsers)
        for k = testSet
            for l = 1:length(selectedFeatures)
                % obtain the user index according to name
                userIdResult = cellfun(@(x) (strcmp(x.username,selectedUsers(j))), testsData{k}(:,i),'UniformOutput',true);
                % get his data
                for r = 1:length(userIdResult)
                    data = eval(['testsData{k}{r,' num2str(selectedGraphs(i)) '}.' selectedFeatures{l}]);
                    testSamples{i,l,j} = [testSamples{i,l,j}; {data}];
                    if userIdResult(r) == 1
                        groundTruth{i,l,j} = [groundTruth{i,l,j} 1];
                    else
                        groundTruth{i,l,j} = [groundTruth{i,l,j} 0];
                    end
                end
            end
        end
    end
end

%% Do procrustes
for i =1: length(selectedGraphs)
    for j = 1:length(selectedFeatures)
        for k = 1:length(selectedUsers)
            if size(positiveSamples{i,j,k}{1},2) ==2
                base = positiveSamples{i,j,k}{1};
                for l = 1:length(positiveSamples{i,j,k})
                    [d,Z] = procrustes(base,positiveSamples{i,j,k}{l},'reflection',false);
                    positiveSamples{i,j,k}{l} = reshape(Z,1,[]);
                end
                for l = 1:length(negativeSamples{i,j,k})
                    [d,Z] = procrustes(base,negativeSamples{i,j,k}{l},'reflection',false);
                    negativeSamples{i,j,k}{l} = reshape(Z,1,[]);
                end
                for l = 1:length(testSamples{i,j,k})
                    [d,Z] = procrustes(base,testSamples{i,j,k}{l},'reflection',false);
                    testSamples{i,j,k}{l} = reshape(Z,1,[]);
                end
            end
        end
    end
end

%% train & test
results = cell(length(selectedGraphs),length(selectedFeatures),length(selectedUsers));
for i =1: length(selectedGraphs)
    for l = 1:length(selectedFeatures)
        for j = 1:length(selectedUsers)
            positive = cell2mat(positiveSamples{i,l,j})';
            negative = cell2mat(negativeSamples{i,l,j})';
            group = [ones(size(positive,2),1); zeros(size(negative,2),1)];
%             SVMModel = fitcsvm([positive'; negative'], group,'Standardize',true,'KernelFunction','RBF',...
%     'KernelScale','auto');
%             result = predict(SVMModel,cell2mat(testSamples{i,l,j}));
            SVMstruct = svmtrain([positive'; negative'], group,'kernel_function','rbf','rbf_sigma',40);
            result = svmclassify(SVMstruct,cell2mat(testSamples{i,l,j}),'Showplot',false)';
            results{i,l,j} = result;
        end
    end
end

%% analyzing result
for j = 1:length(selectedFeatures)
    TP = 0;
    FP = 0;
    TN = 0;
    FN = 0;
    for i =1: length(selectedGraphs)
        
        for k = 1:length(selectedUsers)
            g = groundTruth{i,j,k};
            r = results{i,j,k};
            for l = 1:length(g)
                if g(l) == 1 && r(l) == 1
                    TP = TP +1;
                end
                
                if g(l) == 0 && r(l) == 1
                    FP = FP +1;
                end
                
                if g(l) == 1 && r(l) == 0
                    FN = FN +1;
                end
                
                if g(l) == 0 && r(l) == 0
                    TN = TN +1;
                end
                
            end
        end
    end
%     TOTAL_Sample = TP+FP+TN+FN
    TPR = TP / (TP +FN);
    FPR = FP / (FP+TN);
    
    display([ selectedFeatures(j) ' TPR:' num2str(TPR) ' FPR:' num2str(FPR)]);
end

