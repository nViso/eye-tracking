clc;clear; close all;

eye1 = ParseFusedData('/Users/ZhipingJiang/trackingdata/video_parsing/141115_0112');


%%
errorRate = eye1.reprojectionRMS;
phoneFromO = eye1.vision.tc_vec(errorRate< 4,:);
pLeftCenter =0.5* ( eye1.vision.featureCoordsInFace(1,:) + eye1.vision.featureCoordsInFace(3,:));
pRightCenter =0.5* ( eye1.vision.featureCoordsInFace(2,:) + eye1.vision.featureCoordsInFace(4,:));
pLeftCenter = pLeftCenter / 1000;
pRightCenter = pRightCenter / 1000;
phoneInLeftEye = phoneFromO - ones(length(phoneFromO),1)*pLeftCenter;
phoneInRightEye = phoneFromO - ones(length(phoneFromO),1)*pRightCenter;
phoneInLeftEye = phoneInLeftEye ./ (phoneInLeftEye(:,3) * ones(1,3) );
phoneInRightEye = phoneInRightEye ./ (phoneInRightEye(:,3) * ones(1,3) );

pLeftCenter = pLeftCenter(1:2);
pRightCenter = pRightCenter(1:2);
phoneInLeftEye  = phoneInLeftEye(:,1:2);
phoneInRightEye = phoneInRightEye(:,1:2);
phoneFromO      = phoneFromO(:,1:2);

plx = phoneInLeftEye(:,1);
ply = phoneInLeftEye(:,2);
prx = phoneInRightEye(:,1);
pry = phoneInRightEye(:,2);

gazingL = eye1.vision.leftCenterInFace(errorRate< 4,:);
gazingR = eye1.vision.rightCenterInFace(errorRate< 4,:);
glx = gazingL(:,1);
gly = gazingL(:,2);
grx = gazingR(:,1);
gry = gazingR(:,2);
gazingL(:,3) = [];
gazingR(:,3) = [];

leftXModel = fitlm(gazingL,plx,'RobustOpts','on');
leftYModel = fitlm(gazingL,ply,'RobustOpts','on');

predictX = predict(leftXModel,gazingL);
predictY = predict(leftYModel,gazingL);
priL = [predictX predictY];

figure;coloredScatter(gazingR);
figure;coloredScatter(phoneInLeftEye);
figure;coloredScatter(priL);