baseDir = '/Users/ZhipingJiang/base/results';
userDirs = dir(baseDir);
userDirs(1:3) = [];

datas = {};

for i = 1:length(userDirs)
    currentDir = [baseDir filesep userDirs(i).name];
    groundtruthFiles = dir([currentDir filesep '*.groundtruth']);
    testFiles = dir([currentDir filesep '*.test']);
    assert(length(testFiles) == length(groundtruthFiles), ' the number of files is not matched');
    
    for j = 1:length(testFiles)
        currentTestFile = [currentDir filesep testFiles(j).name];
        currentGTFile = [currentDir filesep groundtruthFiles(j).name];
        currentGT = importdata(currentGTFile);
        currentTest = importdata(currentTestFile);
        currentTest = currentTest(1:size(currentGT),:);
        assert(size(currentGT,1) == size(currentTest,1), ['length not equal,' num2str(i) ' ' num2str(j)]);
        slot.gt = currentGT;
        slot.test = currentTest;
        datas{i,j} = slot;
    end
end

