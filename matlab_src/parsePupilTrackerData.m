function slot = parsePupilTrackerData(currentTestFile)

currentTest = importdata(currentTestFile);
% fill all 0 to nan
currentTest(currentTest == 0) = nan;
currentTest = currentTest(:,1:40);
if ~isempty(find(isnan(currentTest)))
    % interpolate all nan to nearby values.
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
slot.allFeaturePoints = currentTest(:,5:16);
% 17 to 28 is the columns for reprojected points.
slot.reprojectedFeaturePoints = currentTest(:,17:28);
% calculate the differences between detected feature coords and reprojected
% coords.
slot.projectionErrorRMS = rms((slot.allFeaturePoints - slot.reprojectedFeaturePoints)');
slot.tvec             = currentTest(:,29:31);
slot.rvec             = currentTest(:,32:40);
slot.rvec             = reshape(slot.rvec',[3 3 size(slot.rvec,1)]);
% millimeter to meter.
slot.tvec             = slot.tvec /1000;
% this is a self-written function to convert the opencv rvec in axis/angle
% representation into more standard quaternion representation.
slot.quaternion       = dcm2quat(slot.rvec);
% the rvec turns a vector in world coords into camera coords, and this
% rvec_back do the reverse.
slot.rvec_back        = quatinv(slot.quaternion);
% this is the camera position vector in face-center coordinates.
slot.tc_vec           = -quatrotate(slot.rvec_back,slot.tvec);