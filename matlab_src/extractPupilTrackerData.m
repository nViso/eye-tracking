baseDir = '/Users/ZhipingJiang/base/results';
testsDir = dir(baseDir);
testsDir(1:3) = [];

testsData = {};
for k = 1:length(testsDir) 
    disp(['extracting tests: ' testsDir(k).name]);
    currentDir = [baseDir filesep testsDir(k).name];
    userDirs = dir([currentDir filesep] );
    userDirs(1:3) = [];
    
    resultsData = {};
    
    for i = 1:length(userDirs)
        usertestDir = [currentDir filesep userDirs(i).name];
        groundtruthFiles = dir([usertestDir filesep '*.groundtruth']);
        testFiles = dir([usertestDir filesep '*.test']);
        assert(length(testFiles) == length(groundtruthFiles), ' the number of files is not matched');
        
        for j = 1:length(testFiles)
            currentTestFile = [usertestDir filesep testFiles(j).name];
            currentGTFile = [usertestDir filesep groundtruthFiles(j).name];
            currentGT = importdata(currentGTFile);
            currentTest = importdata(currentTestFile);
            currentTest = currentTest(1:size(currentGT),:);
            assert(size(currentGT,1) == size(currentTest,1), ['length not equal,' num2str(i) ' ' num2str(j)]);
            slot.groundtruth = currentGT;
            slot.leftPupilCenter  =  currentTest(:,1:2);
            slot.rightPupilCenter = currentTest(:,3:4);
            slot.leftInnerCanthus = currentTest(:,5:6);
            slot.rightInnerCanthus= currentTest(:,7:8);
            slot.leftOuterCanthus = currentTest(:,9:10);
            slot.rightOuterCanthus= currentTest(:,11:12);
            slot.leftNostrils     = currentTest(:,13:14);
            slot.rightNostrils    = currentTest(:,15:16);
            slot.testname = testsDir(k).name;
            slot.username = userDirs(i).name;
            strsplit(testFiles(j).name,'.');
            slot.tracename = ans{1};
            resultsData{i,j} = slot;
        end
    end
    
    testsData{k} = resultsData;
end
    clearvars -except testsData;
