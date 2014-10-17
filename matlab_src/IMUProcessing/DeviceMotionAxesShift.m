function raw = DeviceMotionAxesShift(raw);
% switch to body coordinate, x for long, y for width, z toward ground.
data = raw.gravity;
data(:,1) = data(:,1) + data(:,2);
data(:,2) = data(:,1) - data(:,2);
data(:,1) = data(:,1) - data(:,2);
data(:,3) = data(:,3);
data = data *9.7935;
raw.gravity = data;

data = raw.acc;
data(:,1) = data(:,1) + data(:,2);
data(:,2) = data(:,1) - data(:,2);
data(:,1) = data(:,1) - data(:,2);
data(:,3) = -data(:,3);
data = data * 9.7935;
raw.acc = data;

data = raw.gyro;
data(:,1) = data(:,1) + data(:,2);
data(:,2) = data(:,1) - data(:,2);
data(:,1) = data(:,1) - data(:,2);
data(:,3) = data(:,3);
raw.gyro = data;

data = raw.mag;
data(:,1) = data(:,1) + data(:,2);
data(:,2) = data(:,1) - data(:,2);
data(:,1) = data(:,1) - data(:,2);
data(:,3) = data(:,3);
raw.mag = data;

dcmfront = [0 1 0; 1 0 0 ; 0 0 1];
dcmback = [0 1 0 ; 1 0 0; 0 0  -1];
data = raw.quat;
dcm = quat2dcm(data);
raw.originaldcm = dcm;
for i = 1:size(dcm,3)
    raw.dcm(:,:,i) = (dcmfront*dcm(:,:,i)*dcmback);
end
raw.quat = dcm2quat(raw.dcm);
[r1 r2 r3] = dcm2angle(raw.dcm);
raw.ori = [r1 r2 r3];

clearvars -except raw;