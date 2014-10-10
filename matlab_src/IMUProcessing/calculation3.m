function calculation3(rawData)
clearvars -except rawData sensorStartTime;


global allstart;
allstart = tic;
global acc
acc= rawData.acc;
global mag;
mag= rawData.mag;
global gyro;
gyro= rawData.gyro;
global ori;
ori= rawData.ori;
global accTimeLine
accTimeLine = rawData.accTimeLine;
global magTimeLine;
magTimeLine= rawData.magTimeLine;
global gyroTimeLine;
gyroTimeLine= rawData.gyroTimeLine;
global oriTimeLine;
oriTimeLine= rawData.oriTimeLine;
global sensorStartTime;
sensorStartTime = rawData.sensorStartTime;
reversed = 0;

NexusGyroTimeLineFix;
accCalibration;
dataAlignment2;

if reversed == 1
    acc = flipud(acc);
    gyro = flipud(gyro);
    gyro = -gyro;
    mag = flipud(mag);
    ori = flipud(ori);
end



motionDetermination;
% StepRecognizer;
%gyroCalibration2;
attitudeByAccMag;
% attitudeByGyro
% AttitudeByGyro2;
%  attitudeEKF2;
oriRagulate;
StepDetector2;
stepMapping;

disp(' All finished');
toc(allstart);
clear allstart;