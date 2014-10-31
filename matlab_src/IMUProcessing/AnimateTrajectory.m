h = Aero.Animation;
h.FramesPerSecond = 20;
h.TimeScaling = 1;
h.createBody('bluewedge.ac','Ac3d');
% h.createBody('redwedge.ac','Ac3d');

data = raw.ori;
timeLine= raw.timeline;
xts = timeseries(data(:,3),timeLine,'name','gyrox');
xts.TimeInfo.Units='nanoseconds';
yts = timeseries(data(:,2),timeLine,'name','gyroy');
yts.TimeInfo.Units='nanoseconds';
zts = timeseries(data(:,1),timeLine,'name','gyroz');
zts.TimeInfo.Units='nanoseconds';
TC = tscollection({xts yts zts});

% 50hz
TC = resample(TC,timeLine(1):0.02:timeLine(end),'linear');
data = [TC.gyrox.Data TC.gyroy.Data TC.gyroz.Data];

cutStart = 5; cutEnd = length(data);
data = data(cutStart:cutEnd,:);
timeLine2 = TC.Time;
timeLine2 = timeLine2(cutStart:cutEnd);
timeLine2 = timeLine2 - timeLine2(1);
flightData = zeros(length(timeLine2),7);
flightData(:,1) =(timeLine2-timeLine2(1));
flightData(:,5:7) = data;
flightData(:,1) = [];
h.Bodies{1}.TimeseriesSource = flightData;
% h.show();
h.play();

clear h flightData ;