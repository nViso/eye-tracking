close all;
addpath('../');
addpath('../IMUProcessing/');

traceName = '/Users/ZhipingJiang/trackingdata/video_parsing/141022_1512';
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
tPhoneNNbyFace = quatrotate(quatFaceToNN,visionData.tc_vec);

%% get phone position through rotating the tvec by sensor.quaternion
tvecBN = quatrotate(quatCfToBN,visionData.tvec);
tFaceNNbySensorOrientation = [];
for i = 1:length(tvecBN)
    sensorIndex = floor(i/length(tvecBN)*length(sensorData.quatBNtoNN));
    tFaceNNbySensorOrientation(i,:) = quatrotate(sensorData.quatBNtoNN(sensorIndex,:),tvecBN(i,:));
end

tPhoneNNbySensorOrientation = - tFaceNNbySensorOrientation;

