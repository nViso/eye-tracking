function alingned = dataAlignment2 (rawData)

acc = rawData.acc;
gyro= rawData.gyro;
mag= rawData.mag;
accTimeLine = rawData.accTimeLine;
magTimeLine = rawData.magTimeLine;
gyroTimeLine= rawData.gyroTimeLine;

[accTimeLine,timeLineIndex] = sort(accTimeLine);
acc(:,:) = acc(timeLineIndex,:);
[gyroTimeLine,timeLineIndex] = sort(gyroTimeLine);
gyro(:,:) = gyro(timeLineIndex,:);
[magTimeLine,timeLineIndex] = sort(magTimeLine);
mag(:,:) = mag(timeLineIndex,:);
% [oriTimeLine,timeLineIndex] = sort(oriTimeLine);
% ori(:,:) = ori(timeLineIndex,:);

% for i = 2:length(gyro)-1
%     if abs(gyro(i,1))>0.06 && abs(gyro(i,1))<0.09 && abs(gyro(i-1,1))<0.02 && abs(gyro(i+1,1))<0.02
%         gyro(i,1) = gyro(i-1,1) + (gyroTimeLine(i)-gyroTimeLine(i-1))/(gyroTimeLine(i)-gyroTimeLine(i+1))*(gyro(i+1,1)-gyro(i-1,1));
%     end
%     if abs(gyro(i,3))>0.06 && abs(gyro(i,3))<0.09 && abs(gyro(i-1,3))<0.02 && abs(gyro(i+1,3))<0.02
%         gyro(i,3) = gyro(i-1,3) + (gyroTimeLine(i)-gyroTimeLine(i-1))/(gyroTimeLine(i)-gyroTimeLine(i+1))*(gyro(i+1,3)-gyro(i-1,3));
%     end
% end


accxts = timeseries(acc(:,1),accTimeLine,'name','accx');
accxts.TimeInfo.Units='seconds';
accyts = timeseries(acc(:,2),accTimeLine,'name','accy');
accyts.TimeInfo.Units='seconds';
acczts = timeseries(acc(:,3),accTimeLine,'name','accz');
acczts.TimeInfo.Units='seconds';
accTC = tscollection({accxts accyts acczts},'name','accTC');

gyroxts = timeseries(gyro(:,1),gyroTimeLine,'name','gyrox');
gyroxts.TimeInfo.Units='seconds';
gyroyts = timeseries(gyro(:,2),gyroTimeLine,'name','gyroy');
gyroyts.TimeInfo.Units='seconds';
gyrozts = timeseries(gyro(:,3),gyroTimeLine,'name','gyroz');
gyrozts.TimeInfo.Units='seconds';
gyroTC = tscollection({gyroxts gyroyts gyrozts});

magxts = timeseries(mag(:,1),magTimeLine,'name','magx');
magxts.TimeInfo.Units='seconds';
magyts = timeseries(mag(:,2),magTimeLine,'name','magy');
magyts.TimeInfo.Units='seconds';
magzts = timeseries(mag(:,3),magTimeLine,'name','magz');
magzts.TimeInfo.Units='seconds';
magTC = tscollection({magxts magyts magzts});

% orixts = timeseries(ori(:,1),oriTimeLine,'name','orix');
% orixts.TimeInfo.Units='nanoseconds';
% oriyts = timeseries(ori(:,2),oriTimeLine,'name','oriy');
% oriyts.TimeInfo.Units='nanoseconds';
% orizts = timeseries(ori(:,3),oriTimeLine,'name','oriz');
% orizts.TimeInfo.Units='nanoseconds';
% oriTC = tscollection({orixts oriyts orizts});


startTime = [accTimeLine(1) gyroTimeLine(1) magTimeLine(1) ];
startTime  = sort(startTime);
startTime = startTime(end);

endTime = [accTimeLine(end) gyroTimeLine(end) magTimeLine(end) ];
endTime  = sort(endTime);
endTime = endTime(1);

%  200Hz resample.
accTC = resample(accTC,startTime:0.005:endTime);
gyroTC = resample(gyroTC,startTime:0.005:endTime,'linear');
magTC = resample(magTC,startTime:0.005:endTime);
% oriTC = resample(oriTC,startTime:5*10e5:endTime);


%  back to normal data series
acc = [accTC.accx.Data accTC.accy.Data accTC.accz.Data];
gyro = [gyroTC.gyrox.Data gyroTC.gyroy.Data gyroTC.gyroz.Data];
mag = [magTC.magx.Data magTC.magy.Data magTC.magz.Data];
% ori = [oriTC.orix.Data oriTC.oriy.Data oriTC.oriz.Data];

cutStart = 1; cutEnd = length(acc);
acc = acc(cutStart:cutEnd,:);
gyro = gyro(cutStart:cutEnd,:);
mag = mag(cutStart:cutEnd,:);
timeLine = accTC.Time;
timeLine = timeLine(cutStart:cutEnd);

for i = 1:length(gyro)
acc(i,4) = norm(acc(i,1:3));
mag(i,4) = norm(mag(i,1:3));
end



clear i;
clear *ts;
clear endTime startTime ;
clear cut*;
clear *TC *TimeLine *Index;

if length(acc) ~= length(gyro)
    clc;
end

alingned.acc = acc;
alingned.gyro = gyro;
alingned.mag = mag;
% alingned.ori = ori;
alingned.timeLine =timeLine;

