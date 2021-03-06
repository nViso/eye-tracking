addpath('./dtw');

for k = 1:length(testsData)
    resultsData = testsData{k};
    for i = 1:size(resultsData,1)
        for j = 1:size(resultsData,2)
            
            slot = resultsData{i,j};
            slot.leftPupilWRTCanthus = slot.leftPupilCenter -  slot.leftInnerCanthus;
            slot.rightPupilWRTCanthus= slot.rightInnerCanthus -slot.rightPupilCenter;
            slot.leftPupilAddCanthus = slot.leftPupilCenter +  slot.leftInnerCanthus;
            slot.rightPupilAddCanthus= slot.rightInnerCanthus +slot.rightPupilCenter;
            groundtruthM = pdist2(slot.groundtruth,slot.groundtruth);
            leftpupilM = pdist2(slot.leftPupilCenter,slot.leftPupilCenter);
            rightpupilM = pdist2(slot.rightPupilCenter,slot.rightPupilCenter);
            leftPwrtCanthusM = pdist2(slot.leftPupilWRTCanthus,slot.leftPupilWRTCanthus);
            rightPwrtCanthusM = pdist2(slot.rightPupilWRTCanthus,slot.rightPupilWRTCanthus);
            slot.leftPupilCenterDTW = simmx(groundtruthM,leftpupilM);
            slot.rightPupilCenterDTW = simmx(groundtruthM,rightpupilM);
            slot.leftPupilWRTCanthusDTW = simmx(leftPwrtCanthusM,rightpupilM);
            slot.rightPupilWRTCanthusDTW = simmx(rightPwrtCanthusM,rightpupilM);
            slot.innerCanthusDifference = slot.leftInnerCanthus - slot.rightInnerCanthus;
            slot.pupilCenterDifference = slot.leftPupilCenter - slot.rightPupilCenter;
            slot.pupilCenterDifferenceRatio = slot.pupilCenterDifference./(slot.innerCanthusDifference);
            resultsData{i,j} = slot;
        end
    end
    testsData{k} = resultsData;
end
clearvars -except testsData;