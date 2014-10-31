%% Turn the iOS right-front-up right_hand_ruled sensor cooridnate system 
%  into more standard North-East-Down right_hand_ruled coordinate system.
function result = DeviceMotionAxesShift(raw)
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
data = data * -9.7935;
raw.acc = data;

data = raw.gyro;
data(:,1) = data(:,1) + data(:,2);
data(:,2) = data(:,1) - data(:,2);
data(:,1) = data(:,1) - data(:,2);
data(:,3) = -data(:,3);
raw.gyro = data;

data = raw.mag;
data(:,1) = data(:,1) + data(:,2);
data(:,2) = data(:,1) - data(:,2);
data(:,1) = data(:,1) - data(:,2);
data(:,3) = -data(:,3);
raw.mag = data;

% the iOS is north-west-up, turn to North-earth-down.
q_NiOSToNN = dcm2quat([1 0 0 ; 0 -1 0 ; 0 0 -1]);
% the iOS is right-front-up axis, to front-right-down 
q_BNToBiOS = dcm2quat([0 1 0; 1 0 0; 0 0 -1]);

% after these two rotation, turn the navigation_b (FRD) to navigation_n
% (NED)
quat = quatmultiply(q_BNToBiOS,raw.quatBiOStoNiOS);
quat = quatmultiply(quat,q_NiOSToNN);

raw.quatBNtoNN = quat;
raw.quatNNtoBN = quatinv(quat);

[r3 r2 r1] = quat2angle(raw.quatNNtoBN);
raw.ori = [r1 r2 r3];
result = raw;