videoFileName = '/Users/ZhipingJiang/trackingdata/video_parsing/141116_0329vision.mov';
metaFileName  = '/Users/ZhipingJiang/trackingdata/video_parsing/141116_0329vision.test';
metaInfo = parsePupilTrackerData(metaFileName);
blue = uint8([0 0 255]); % [R G B]; class of yellow must match class of I
green = uint8([0 255 0]); % [R G B]; class of yellow must match class of I
newCircleDraw = vision.MarkerInserter('Shape','Plus','Size',20,'BorderColor','Custom','CustomBorderColor',[0 255 0]);
oldCircleDraw = vision.MarkerInserter('Shape','Plus','Size',20,'BorderColor','Custom','CustomBorderColor',[0 0 255]);
featurePointsDraw = vision.MarkerInserter('Shape','Circle','Size',5,'BorderColor','Custom','CustomBorderColor',[255 255 255]);
videoFReader = vision.VideoFileReader(videoFileName,'VideoOutputDataType','uint8');
videoPlayer = vision.VideoPlayer;
fn =0;
while ~isDone(videoFReader)
  videoFrame = step(videoFReader);
  fn = fn +1;
  li = metaInfo.leftInnerCanthus(fn,:);
  ri = metaInfo.rightInnerCanthus(fn,:);
  lo = metaInfo.leftOuterCanthus(fn,:);
  ro = metaInfo.rightOuterCanthus(fn,:);
  lc = round((li + lo) /2);
  rc = round((ri + ro) /2);
  lhd = round(abs(lo(1) - li(1))/2);
  rhd = round(abs(ro(1) - ri(1))/2);
  ltl = [lc(1) - lhd lc(2) - lhd];
  lbr = [lc(1) + lhd lc(2) + lhd];
  rtl = [rc(1) - rhd rc(2) - rhd];
  rbr = [rc(1) + rhd rc(2) + rhd];
  leftEye = videoFrame(ltl(2):lbr(2),ltl(1):lbr(1),:);
  rightEye = videoFrame(rtl(2):rbr(2),rtl(1):rbr(1),:);
  [ler, lec] = LocatePupilCenter(leftEye);
  [rer, rec] = LocatePupilCenter(rightEye);
  newLeft = ltl + [ lec ler];
  newRight = rtl + [ rec rer];
  oldLeft = metaInfo.leftPupilCenter(fn,:);
  oldRight= metaInfo.rightPupilCenter(fn,:);
  featurePoints = int32(reshape(metaInfo.allFeaturePoints(fn,:),[2 7])');
  newCirles = int32(round([newLeft ; newRight]));
  oldCirles = int32([oldLeft ; oldRight]);
  videoFrame= step(newCircleDraw,videoFrame,newCirles);
  videoFrame = step(oldCircleDraw,videoFrame,oldCirles);
  videoFrame = step(featurePointsDraw,videoFrame,featurePoints);
  
  step(videoPlayer,videoFrame);
end
release(videoPlayer);
release(videoFReader);