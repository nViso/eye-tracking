raw = sensorTextReadFromDisk('sensor.txt');
DeviceMotionAxesShift;

%% filtering
HhpSpec = fdesign.highpass(0.3,0.5,60,1,raw.samplingRate);
Hhp = design(HhpSpec);
[delay, ~] = phasedelay(Hhp);
delay = median(delay(isnan(delay)==false));

raw.acc = filter(Hhp,[raw.acc zeros(size(raw.acc,1),delay)],2);
raw.gyro= filter(Hhp,[raw.gyro zeros(size(raw.gyro,1),delay)],2);
raw.mag = filter(Hhp,[raw.mag zeros(size(raw.mag,1),delay)],2);
raw.acc = raw.acc(:,delay+1:end);
raw.gyro = raw.gyro(:,delay+1:end);
raw.mag = raw.mag(:,delay+1:end);

%% user acceleration in navigation frame

acc_n = zeros(3,size(raw.acc,2));
for i = 1:size(raw.acc,2)
    acc_n(1:3,i) = raw.dcm(:,:,i)' * raw.acc(1:3,i);
end
acc_n = filter(Hhp,[acc_n zeros(3,delay)],2);
acc_n = acc_n(:,delay+1:end);
%% acceleration integration via velocity verlet

verletPos = zeros(size(acc_n));
verletSpeed = zeros(size(acc_n));
for i = 3:size(acc_n,2)
    verletPos(:,i) = 2*verletPos(:,i-1) - verletPos(:,i-2) + acc_n(:,i-1)*(1/raw.samplingRate)^2;
    verletSpeed(:,i-1) = ( verletPos(:,i) - verletPos(:,i-2) ) / (2 * (1/raw.samplingRate) );
end
verletSpeed(:,end) = verletSpeed(:,end-1);
verletPos = verletPos';
verletSpeed = verletSpeed';

plot3(verletPos(:,1),verletPos(:,2),verletPos(:,3));

%% accleration via velocity decay model

tau = 0.9;

speed = zeros(size(acc_n));
pos   = zeros(size(acc_n));
for i = 2 : size(speed,2)
    speed(:,i) = speed(:,i-1) + acc_n(:,i)*(1/raw.samplingRate) * tau;
    pos(:,i) = pos(:,i-1) + speed(:,i) *(1/raw.samplingRate);
end