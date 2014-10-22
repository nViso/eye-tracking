close all;
addpath('../');
addpath('../IMUProcessing/');

traceName = '/Users/ZhipingJiang/trackingdata/video_parsing/141022_1636';
visionTraceName = [traceName 'vision.test'];
sensorTraceName = [traceName 'sensor.txt'];

sensorData = sensorTextReadFromDisk(sensorTraceName);
sensorData = DeviceMotionAxesShift(sensorData);
visionData = parsePupilTrackerData(visionTraceName);

clearvars -except *Data;

q_cfToBN = dcm2quat([0 -1 0; -1 0 0; 0 0 -1]);
q_faceToNN  = zeros(size(visionData.rvec_back));
q_faceToBN  = zeros(size(visionData.rvec_back));

for i = 1:length(visionData.quaternion)
    sensorIndex = floor(i/length(visionData.quaternion)*length(sensorData.quatBNtoNN));
    q_faceToBN(i,:) = quatmultiply(visionData.quaternion(i,:),q_cfToBN);
    q_faceToNN(i,:) = quatmultiply(q_faceToBN(i,:),sensorData.quatBNtoNN(sensorIndex,:));
end
output = quatrotate(q_faceToNN,[0 0 1]);

showAxesDirection(q_faceToNN);
figure;scatter3(visionData.tc_vec(:,1),visionData.tc_vec(:,2),visionData.tc_vec(:,3),50./visionData.projectionErrorRMS,1:length(visionData.tc_vec),'filled');
figure;scatter3(output(:,1),output(:,2),output(:,3),50./visionData.projectionErrorRMS,1:length(output),'filled');