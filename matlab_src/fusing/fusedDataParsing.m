error('This script is deprecated. User ParseFusedData function instead');

clc;clear;
addpath('../');
addpath('../IMUProcessing/');

traceName = '/Users/ZhipingJiang/trackingdata/video_parsing/141027_1123';
visionTraceName = [traceName 'vision.test'];
sensorTraceName = [traceName 'sensor.txt'];

sensorData = sensorTextReadFromDisk(sensorTraceName);
sensorData = DeviceMotionAxesShift(sensorData);
visionData = parsePupilTrackerData(visionTraceName);

clearvars -except *Data;

%% get the rotation from face-centered coords to navigational NED coords.
quatCfToBN = dcm2quat([0 -1 0; -1 0 0; 0 0 -1]);
quatFaceToNN  = zeros(size(visionData.rvec_back));
quatFaceToBN  = zeros(size(visionData.rvec_back));
quatSensorBNToBN  = zeros(size(visionData.rvec_back));
for i = 1:length(visionData.quaternion)
    sensorIndex = floor(i/length(visionData.quaternion)*length(sensorData.quatBNtoNN));
    quatSensorBNToBN(i,:) = sensorData.quatBNtoNN(sensorIndex,:);
    quatFaceToBN(i,:) = quatmultiply(visionData.quaternion(i,:),quatCfToBN);
    quatFaceToNN(i,:) = quatmultiply(quatFaceToBN(i,:),sensorData.quatBNtoNN(sensorIndex,:));
end

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
[r3 r2 r1] = quat2angle(sensorData.quatBNtoNN);
phoneAngleNN = rad2deg(unwrap([r1 r2 r3]));
[r3 r2 r1] = quat2angle(quatFaceToBN);
faceAngleBN = rad2deg(unwrap([r1 r2 r3]));
[r3 r2 r1] = quat2angle(quatFaceToNN);
faceAngleNN = rad2deg(unwrap([r1 r2 r3]));
clear r*;

%% calculate the error correction rotation if head not moving.
if sum(std(faceAngleNN)) < 25
    quatFaceToNNIdeal = angle2quat(deg2rad(median(faceAngleNN(:,3))),deg2rad(median(faceAngleNN(:,2))),deg2rad(median(faceAngleNN(:,1))));
    quatFaceToNNIdeal = ones(length(quatFaceToNN),1) * quatFaceToNNIdeal;
    
    m1 = quatmultiply(quatinv(visionData.quaternion),quatFaceToNNIdeal);
    m2 = quatmultiply(m1,quatinv(quatSensorBNToBN));
    quatError = quatmultiply(m2,quatinv(quatCfToBN));
    
    [r3 r2 r1] = quat2angle(quatError);
    errorAngle = rad2deg(unwrap([r1 r2 r3]));
end