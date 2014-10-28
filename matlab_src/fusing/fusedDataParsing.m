close all;
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

for i = 1:length(visionData.quaternion)
    sensorIndex = floor(i/length(visionData.quaternion)*length(sensorData.quatBNtoNN));
    quatFaceToBN(i,:) = quatmultiply(visionData.quaternion(i,:),quatCfToBN);
    quatFaceToNN(i,:) = quatmultiply(quatFaceToBN(i,:),sensorData.quatBNtoNN(sensorIndex,:));
end

%% get phone position through rotating the tv_vec by quatFaceToNN
% this is actually R_bn^nn * R_cf^bn * V_cf(tvec)
tPhoneNNbyFace = quatrotate(quatFaceToNN,visionData.tc_vec);

%% get phone position through rotating the tvec by sensor.quaternion
% this is R_bn^nn *R_cf^bn * R_fe^cf * ( -R_cf*fe * V_cf(tvec) )
% both this two forms are mathmatically identical!!!!
tvecBN = quatrotate(quatCfToBN,visionData.tvec);
tFaceNNbySensorOrientation = [];
for i = 1:length(tvecBN)
    sensorIndex = floor(i/length(tvecBN)*length(sensorData.quatBNtoNN));
    tFaceNNbySensorOrientation(i,:) = quatrotate(sensorData.quatBNtoNN(sensorIndex,:),tvecBN(i,:));
end

tPhoneNNbySensorOrientation = - tFaceNNbySensorOrientation;

%%
densePhoneNNByFace = interp1([0:sensorData.timeline(end)/length(tPhoneNNbyFace):sensorData.timeline(end)-sensorData.timeline(end)/length(tPhoneNNbyFace)]',tPhoneNNbyFace,sensorData.timeline);
densePhoneSpeedNNByFace = [0 0 0; diff(densePhoneNNByFace)];
densePhoneAccNNByFace = [0 0 0; diff(densePhoneSpeedNNByFace)];
sensorAccNN = quatrotate(sensorData.quatBNtoNN,sensorData.acc(:,1:3));
accDiff = sensorAccNN - densePhoneAccNNByFace;