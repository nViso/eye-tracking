function slot = parsePupilTrackerData(currentTestFile,gap1Model,gap2Model,gap3Model)

withModel = false;

if nargin == 4
    withModel = true;
    raw = [gap1Model.Variables.x1 gap1Model.Variables.x2 gap1Model.Variables.x3];
    peak = evaluate(kde(raw',[0.1 0.1 0.1]'),median(raw)');
    threashold = 0.1;
end

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
% millimeter to meter.
slot.tvec             = slot.tvec /1000;
% 32 to 40 is the rotation matrix in column-first order.
slot.rvec             = currentTest(:,32:40);
slot.rvec             = reshape(slot.rvec',[3 3 size(slot.rvec,1)]);
slot.quatFaceToC       = dcm2quat(slot.rvec);
if withModel == true
    [r(:,1) r(:,2) r(:,3)] = (quat2angle(slot.quatFaceToC));
    % this unwrap is crucial to error fixing accuracy.
    r = unwrap(r);
    [g1 c1] = predict(gap1Model,r);
    [g2 c2] = predict(gap2Model,r);
    [g3 c3] = predict(gap3Model,r);
    centrality =  evaluate(kde(raw',[0.1 0.1 0.1]'),r')';
    centrality(centrality < peak * threashold) = 0;
    centrality = centrality/peak;
    centrality = centrality.^0.3;
    g1 = g1 .* centrality;
    g2 = g2 .* centrality;
    g3 = g3 .* centrality;
    quatFix = angle2quat(g1,g2,g3);
    slot.quatFaceToC = quatmultiply(slot.quatFaceToC,quatFix);
end
% the rvec turns a vector in world coords into camera coords, and this
% rvec_back do the reverse.
slot.quatCtoFace        = quatinv(slot.quatFaceToC);
% this is the camera position vector in face-center coordinates.
slot.tc_vec           = -quatrotate(slot.quatCtoFace,slot.tvec);