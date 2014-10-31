function pack = ParseFusedData(pathHeader,gap1Model,gap2Model,gap3Model,density,threshold)

withModel = false;

if nargin == 4
    withModel = true;
end

addpath('../');
addpath('../IMUProcessing/');

traceName = pathHeader;
visionTraceName = [traceName 'vision.test'];
sensorTraceName = [traceName 'sensor.txt'];

sensorData = sensorTextReadFromDisk(sensorTraceName);
sensorData = DeviceMotionAxesShift(sensorData);
if withModel == false
    visionData = parsePupilTrackerData(visionTraceName);
else 
    visionData = parsePupilTrackerData(visionTraceName,gap1Model,gap2Model,gap3Model);
end

clearvars -except *Data;

%% get the rotation from face-centered coords to navigational NED coords.
quatCfToBN = dcm2quat([0 -1 0; -1 0 0; 0 0 -1]);
quatFaceToNN  = zeros(size(visionData.quatFaceToC));
quatFaceToBN  = zeros(size(visionData.quatFaceToC));
quatSensorBNToNN  = zeros(size(visionData.quatFaceToC));
for i = 1:length(visionData.quatFaceToC)
    sensorIndex = floor(i/length(visionData.quatFaceToC)*length(sensorData.quatBNtoNN));
    quatSensorBNToNN(i,:) = sensorData.quatBNtoNN(sensorIndex,:);
end
quatFaceToBN = quatmultiply(visionData.quatFaceToC,quatCfToBN);
quatFaceToNN = quatmultiply(quatFaceToBN,quatSensorBNToNN);
%% get phone position through rotating the tv_vec by quatFaceToNN
% this is actually R_bn^nn * R_cf^bn * V_cf(tvec)
tPhoneNNbyFace = quatrotate(quatFaceToNN,visionData.tc_vec);

%% get phone position through rotating the tvec by sensor.quaternion
% this is R_bn^nn *R_cf^bn * R_fe^cf * ( -R_cf*fe * V_cf(tvec) )
% both these two forms are mathmatically identical!!!!
tvecBN = quatrotate(quatCfToBN,visionData.tvec);
tFaceNNbySensorOrientation = [];
for i = 1:length(tvecBN)
    sensorIndex = floor(i/length(tvecBN)*length(sensorData.quatBNtoNN));
    tFaceNNbySensorOrientation(i,:) = quatrotate(sensorData.quatBNtoNN(sensorIndex,:),tvecBN(i,:));
end

tPhoneNNbySensorOrientation = - tFaceNNbySensorOrientation;

%% three frames in angle.
[r1 r2 r3] = quat2angle(sensorData.quatBNtoNN);
phoneAngleNN = rad2deg(unwrap([r1 r2 r3]));
[r1 r2 r3] = quat2angle(visionData.quatFaceToC);
faceAngleC = rad2deg(unwrap([r1 r2 r3]));
[r1 r2 r3] = quat2angle(quatFaceToBN);
faceAngleBN = rad2deg(unwrap([r1 r2 r3]));
[r1 r2 r3] = quat2angle(quatFaceToNN);
faceAngleNN = rad2deg(unwrap([r1 r2 r3]));
clear r*;

%% calculate the error correction rotation if head not moving.

quatFaceToNNIdeal = angle2quat(deg2rad(median(faceAngleNN(:,1))),deg2rad(median(faceAngleNN(:,2))),deg2rad(median(faceAngleNN(:,3))));
quatFaceToNNIdeal = ones(length(quatFaceToNN),1) * quatFaceToNNIdeal;

m1 = quatmultiply(quatinv(visionData.quatFaceToC),quatFaceToNNIdeal);
m2 = quatmultiply(m1,quatinv(quatSensorBNToNN));
quatError = quatmultiply(m2,quatinv(quatCfToBN));
% this quatError has been verified by following code.
% show3DAxesDirection(quatFaceToNN) % old faceNN.
% visionData.quatFaceToC = quatmultiply(visionData.quatFaceToC,quatError);
% quatFaceToBN = quatmultiply(visionData.quatFaceToC,quatCfToBN);
% quatFaceToNN = quatmultiply(quatFaceToBN,quatSensorBNToNN);
% show3DAxesDirection(quatFaceToNN) % new faceNN, should be gethered.

[r1 r2 r3] = quat2angle(quatError);
errorAngle = rad2deg(unwrap([r1 r2 r3]));
% this code has been verified by code:
% eangle2 = deg2rad(errorAngle);
% quate2 = angle2quat(eangle2(:,1),eangle2(:,2),eangle2(:,3));
% visionData.quatFaceToC = quatmultiply(visionData.quatFaceToC,quate2);
% quatFaceToBN = quatmultiply(visionData.quatFaceToC,quatCfToBN);
% quatFaceToNN = quatmultiply(quatFaceToBN,quatSensorBNToNN);
% show3DAxesDirection(quatFaceToNN) % new faceNN, should be gethered.

%% packaging
pack.quatFaceToBN = quatFaceToBN;
pack.quatFaceToNN = quatFaceToNN;
pack.quatSensorBNToNN = quatSensorBNToNN;
pack.quatFaceToNNIdeal = quatFaceToNNIdeal;
pack.quatSensorVisionGap = quatError;
pack.phoneAngleNN = phoneAngleNN;
pack.faceAngleBN = faceAngleBN;
pack.faceAngleNN = faceAngleNN;
pack.faceAngleC  = faceAngleC;
pack.gapAngle = errorAngle;
pack.headMobility = sum(std(faceAngleNN));
pack.reprojectionRMS = visionData.projectionErrorRMS;