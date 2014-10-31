raw = sensorTextReadFromDisk('/Users/ZhipingJiang/trackingdata/video_parsing/141017_1406sensor.txt');
raw = DeviceMotionAxesShift(raw);

%% filtering
HhpSpec = fdesign.highpass(0.01,0.14,60,1,raw.samplingRate);
Hhp = design(HhpSpec);
[delay, ~] = phasedelay(Hhp);
delay = median(delay(isnan(delay)==false));

raw.acc = filter(Hhp,[raw.acc; zeros(delay,size(raw.acc,2))],1);
raw.gyro= filter(Hhp,[raw.gyro; zeros(delay,size(raw.gyro,2))],1);
raw.mag = filter(Hhp,[raw.mag; zeros(delay,size(raw.mag,2))],1);
raw.acc = raw.acc(delay+1:end,:);
raw.gyro = raw.gyro(delay+1:end,:);
raw.mag = raw.mag(delay+1:end,:);

%% user acceleration in navigation frame

acc_n = zeros(size(raw.acc,1),3);
for i = 1:size(raw.acc,1)
%     acc_n(i,:) = raw.dcm(:,:,i)' * raw.acc(i,1:3)';
    acc_n(i,:) = quatrotate(raw.quatBNtoNN(i,:), raw.acc(i,1:3));
end
acc_n = filter(Hhp,[acc_n; zeros(delay,size(acc_n,2))],1);
acc_n = acc_n(delay+1:end,:);
%% acceleration integration via velocity verlet

verletPos = zeros(size(acc_n));
verletSpeed = zeros(size(acc_n));
for i = 3:size(acc_n,1)
    verletPos(i,:) = 2*verletPos(i-1,:) - verletPos(i-2,:) + acc_n(i-1,:)*(1/raw.samplingRate)^2;
end
verletSpeed = diff(verletPos);

plot3(verletPos(:,1),verletPos(:,2),verletPos(:,3));

%% accleration via velocity decay model

tau = 0.9;

speed = zeros(size(acc_n));
pos   = zeros(size(acc_n));
for i = 2 : size(speed,1)
    speed(i,:) = speed(i-1,:) + acc_n(i,:)*(1/raw.samplingRate) * tau;
    pos(i,:) = pos(i-1,:) + speed(i,:) *(1/raw.samplingRate);
end

figure;plot3(pos(:,1),pos(:,2),pos(:,3));