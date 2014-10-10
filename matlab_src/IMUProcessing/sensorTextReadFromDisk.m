function raw = sensorTextReadFromDisk(filepath)

format long;
digits(64);
format long;
fid = fopen(filepath,'rt');

DS = textscan(fid,'%s %n %n %n %n %n');
fclose(fid);
accLength = 0;
gravityLength = 0;
magLength = 0;
gyroLength =0;
quatLength = 0;

for i = 1:length(DS{1})
    type = DS{1}(i);
    switch type{1}(1)
        case 'a'
            accLength = accLength +1;
        case 'e'
            gravityLength = gravityLength+1;
        case 'g'
            gyroLength = gyroLength +1;
        case 'm'
            magLength = magLength+1;
        case 'q'
            quatLength = quatLength +1;
    end
end
acc = zeros(accLength,4);
gravity = zeros(gravityLength,3);
mag = zeros(magLength,4);
gyro = zeros(gyroLength,3);
quat = zeros(quatLength,4);
%rv = zeros(rvLength,5);
gyroTimeLine= zeros(gyroLength,1);
accTimeLine = zeros(accLength,1);
gravityTimeLine = zeros(gravityLength,1);
% oriTimeLine = zeros(oriLength,1);
magTimeLine = zeros(magLength,1);
quatTimeLine =zeros(quatLength,1);
%rvTimeLine = zeros(rvLength,1);

accCount = 1;magCount = 1; gyroCount = 1; quatCount = 1; gravityCount = 1;

matrix = cell2mat(DS(2:6));
for i = 1:size(matrix,1)
    type = DS{1}(i);
    data = matrix(i,1:4);
    time = matrix(i,5);
    
        switch type{1}(1)
        case 'a'
            all = norm(data(1:3));
            acc(accCount,:) = [data(1:3) all];
            accTimeLine(accCount)=time;
            accCount = accCount+1;
        case 'e'
            gravity(gravityCount,:) = data(1:3);
            gravityTimeLine(gravityCount)=time;
            gravityCount = gravityCount +1;
        case 'g'
            gyro(gyroCount,:) = data(1:3);
            gyroTimeLine(gyroCount)=time;
            gyroCount = gyroCount +1;
        case 'm'
            all = norm(data(1:3));
            mag(magCount,:) = [data(1:3) all];
            magTimeLine(magCount)=time;
            magCount = magCount+1;
        case 'q'
            quat(quatCount,:) = data(1:4);
            quatTimeLine(quatCount) = time;
            quatCount = quatCount +1;
        end
end

acc =acc(10:end,:);
gravity = gravity(10:end,:);
gyro = gyro(10:end,:);
mag = mag(10:end,:);
quat = quat(10:end,:);
accTimeLine = accTimeLine(10:end,:);

rawData.acc = acc;
rawData.gravity = gravity;
rawData.gyro = gyro;
rawData.mag = mag;
rawData.quat = quat;
[r1 r2 r3] = quat2angle(quat);
rawData.ori = [r1 r2 r3];
accTimeLine = accTimeLine - accTimeLine(1);
rawData.timeline = accTimeLine;
rawData.interval = median(diff(accTimeLine));
rawData.samplingRate = 1/rawData.interval;

clear *Length *Count;
clearvars -except sensorStart* gyro* acc* mag* ori* rv* operationTime rawData filename;


clock;

clear fileName;
clear ans;

raw = rawData;

