function slot = parsePupilTrackerData(currentTestFile,gap1Model,gap2Model,gap3Model)

withModel = false;

if nargin == 4
    withModel = true;
    raw = [gap1Model.Variables.x1 gap1Model.Variables.x2 gap1Model.Variables.x3];
    peak = evaluate(kde(raw',[0.1 0.1 0.1]'),median(raw)');
    threashold = 0.1;
end

columns = 80;

fileID = fopen(currentTestFile);
format = [];
for i = 1:columns
    format = [format '%n '];
end
format(end) = [];
currentTest = textscan(fileID,format);
currentTest = cell2mat(currentTest);
fclose(fileID);

currentTest = currentTest(:,1:columns);
% fill all 0 to nan
currentTest(currentTest == 0) = nan;
currentTest(:,sum(isnan(currentTest)) == size(currentTest,1)) = 0;
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
slot.midLip           = currentTest(:,17:18);
slot.allFeaturePoints = currentTest(:,5:18);
slot.reprojectedFeaturePoints = currentTest(:,19:32);
slot.cameraMatrix     = currentTest(:,33:41);
slot.cameraMatrix = unique(slot.cameraMatrix,'rows');
slot.cameraMatrix = reshape(slot.cameraMatrix,[3 3]);
slot.tvec             = currentTest(:,42:44);
slot.rvec             = currentTest(:,45:53);
slot.leftCenterInFace = currentTest(:,54:56);
slot.rightCenterInFace = currentTest(:,57:59);
slot.featureCoordsInFace = currentTest(:,60:end);
slot.featureCoordsInFace = unique(slot.featureCoordsInFace,'rows');

slot.featureCoordsInFace = reshape(slot.featureCoordsInFace,[3 7])';

slot.glabellaPoint    = (slot.leftInnerCanthus + slot.rightInnerCanthus)/2;
slot.midNosePoint     = (slot.leftNostrils + slot.rightNostrils)/2;
slot.vectorNoseEye    = slot.midNosePoint-slot.glabellaPoint;
slot.vectorLipEye     = slot.midLip-slot.glabellaPoint;
slot.vectorLipNose    = slot.midLip-slot.midNosePoint;
slot.noseRatio        = sqrt(sum(slot.vectorNoseEye.^2,2)) ./ sqrt(sum(slot.vectorLipNose.^2,2));
slot.noseAngle        = rad2deg(acos(sum(slot.vectorNoseEye.*slot.vectorLipNose,2)./ sqrt(sum(slot.vectorNoseEye.^2,2)) ./sqrt(sum(slot.vectorLipNose.^2,2)) ));

vliri = sqrt(sum((slot.leftInnerCanthus - slot.rightInnerCanthus).^2,2));
vloro = sqrt(sum((slot.leftOuterCanthus - slot.rightOuterCanthus).^2,2));
vlom = sqrt(sum((slot.leftOuterCanthus - slot.midLip).^2,2));
vrom = sqrt(sum((slot.rightOuterCanthus - slot.midLip).^2,2));
vlim = sqrt(sum((slot.leftInnerCanthus - slot.midLip).^2,2));
vrim = sqrt(sum((slot.rightInnerCanthus - slot.midLip).^2,2));
vgm = sqrt(sum((slot.glabellaPoint - slot.midLip).^2,2));
vlog = sqrt(sum((slot.glabellaPoint - slot.leftOuterCanthus).^2,2));
vrog = sqrt(sum((slot.glabellaPoint - slot.rightOuterCanthus).^2,2));
vgn  = sqrt(sum((slot.midNosePoint - slot.glabellaPoint).^2,2));
vlon = sqrt(sum((slot.midNosePoint - slot.leftOuterCanthus).^2,2));
vron = sqrt(sum((slot.midNosePoint - slot.rightOuterCanthus).^2,2));

slorom = 0.5*(vloro+vlom+vrom)./vloro;
slirim = 0.5*(vliri+vlim+vrim)./vliri;
slogm = 0.5*(vlom+vgm+vlog)./vlog;
srogm = 0.5*(vrom+vgm+vrog)./vrog;
sloron = 0.5*(vloro+vlon+vron)./vloro;
slogn = 0.5*(vlon+vgn+vlog)./vlog;
srogn = 0.5*(vron+vgn+vrog)./vrog;

slot.heronNoseArea = sqrt(sloron.*(sloron-vloro./vloro).*(sloron-vlon./vloro).*(sloron-vron./vloro));
slot.heronNoseLeft = sqrt(slogn.*(slogn-vlon./vlog).*(slogn-vgn./vlog).*(slogn-vlog./vlog));
slot.heronNoseRight = sqrt(srogn.*(srogn-vron./vrog).*(srogn-vgn./vrog).*(srogn-vrog./vrog));
areaRatioSmall = 0.5*(slot.heronNoseLeft./slot.heronNoseRight + slot.heronNoseRight./slot.heronNoseLeft);

slot.heronFaceArea = sqrt(slorom.*(slorom-vloro./vloro).*(slorom-vlom./vloro).*(slorom-vrom./vloro));
slot.heronFaceAreaLeft = sqrt(slogm.*(slogm-vlom./vlog).*(slogm-vgm./vlog).*(slogm-vlog./vlog));
slot.heronFaceAreaRight = sqrt(srogm.*(srogm-vrom./vrog).*(srogm-vgm./vrog).*(srogm-vrog./vrog));

slot.heronAreaComperhensive = slot.heronFaceArea + slot.heronNoseArea + slot.heronNoseLeft*0.25 + slot.heronNoseRight*0.25+ slot.heronFaceAreaLeft*0.25 + slot.heronFaceAreaRight *0.25;
areaRatioBig = 0.5*(slot.heronFaceAreaLeft./slot.heronFaceAreaRight + slot.heronFaceAreaRight./slot.heronFaceAreaLeft);
slot.symmetryScore = (areaRatioBig+areaRatioSmall-2)*30;


slot.edgesRatioLarge       = vloro./vlom + vlom./vloro + vloro./vrom + vrom./vloro + vlom./vrom + vrom./vlom + ...
                             vloro./vlon + vlon./vloro + vloro./vron + vron./vloro + vlon./vron + vron./vlon ;
% 17 to 28 is the columns for reprojected points.

% calculate the differences between detected feature coords and reprojected
% coords.
slot.projectionErrorRMS = rms((slot.allFeaturePoints - slot.reprojectedFeaturePoints)');
% millimeter to meter.
slot.tvec             = slot.tvec /1000;
% 32 to 40 is the rotation matrix in column-first order.
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

slot.testFileName = currentTestFile;
slot.videoFileName = regexprep(slot.testFileName,'vision\.test.*','vision\.mov');