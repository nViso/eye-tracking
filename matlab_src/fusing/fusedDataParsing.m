close all;
addpath('../');
addpath('../IMUProcessing/');

traceName = '/Users/ZhipingJiang/trackingdata/video_parsing/141019_1137';
visionTraceName = [traceName 'vision.test'];
sensorTraceName = [traceName 'sensor.txt'];

sensorData = sensorTextReadFromDisk(sensorTraceName);
visionData = parsePupilTrackerData(visionTraceName);

clearvars -except *Data;

q = visionData.quaternion;
t = visionData.tvec;
s = 20*(1./visionData.projectionErrorRMS);
% t(:,1) = - t(:,1);
% t = t(1:400,:);
% q = q(1:400,:);
dcm = quat2dcm(q);

% c = zeros(size(t));
% for i = 1 :length(t)
%     c(i,:) = (dcm(:,:,i)'*(t(i,:))')';
% end

qinverse = quatinv(q);
c = -quatrotate(qinverse,t);

figure1= figure;
axes1= axes('Parent',figure1,'DataAspectRatio',[1 1 1]);
grid(axes1,'on');
hold(axes1,'all');
scatter3(t(:,1),t(:,2),t(:,3),s,[1:length(t)]','filled');
figure2 = figure;
axes2 = axes('Parent',figure2,'DataAspectRatio',[1 1 1]);
grid(axes2,'on');
hold(axes2,'all');
scatter3(c(:,1),c(:,2),c(:,3),s,[1:length(c)]','filled');