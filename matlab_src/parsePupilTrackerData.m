function slot = parsePupilTrackerData(currentTestFile)

currentTest = importdata(currentTestFile);
currentTest(currentTest == 0) = nan;
currentTest = currentTest(:,1:22);
if ~isempty(find(isnan(currentTest)))
    [row, col] = find(isnan(currentTest));
    currentTest(:,unique(col)) = interp1(setdiff(1:size(currentTest,1),unique(row))',currentTest(setdiff(1:size(currentTest,1),unique(row))',unique(col)),1:size(currentTest,1),'nearest','extrap');
end

slot.leftPupilCenter  =  currentTest(:,1:2);
slot.rightPupilCenter = currentTest(:,3:4);
slot.leftInnerCanthus = currentTest(:,5:6);
slot.rightInnerCanthus= currentTest(:,7:8);
slot.leftOuterCanthus = currentTest(:,9:10);
slot.rightOuterCanthus= currentTest(:,11:12);
slot.leftNostrils     = currentTest(:,13:14);
slot.rightNostrils    = currentTest(:,15:16);
slot.tvec             = currentTest(:,17:19);
slot.rvec             = currentTest(:,20:22);
slot.quaternion       = AxisAngleToQuaternion(slot.rvec);