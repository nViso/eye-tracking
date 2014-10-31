tic
global transformGyro2;
transformGyro2 = zeros(length(gyro),4) ;
transformGyro2(1,:) = [1 0 0 0];

for i = 2 : length(gyro)
%     if motionState(i) <0.5
%         transformGyro2(i,:) = transformGyro2(i-1,:);
%     else 
    
    wx = gyro(i,2);
    wy = gyro(i,1);
    wz =- gyro(i,3);
    
    omega = [0  -wx -wy -wz;
             wx   0  wz -wy;
             wy -wz   0  wx;
             wz  wy -wx   0;];
         
    omega = omega *0.5 *(timeLine(i) - timeLine(i-1))/1e9;
    omega = omega + eye(4);
    
    transformGyro2(i,:) = ( omega*transformGyro2(i-1,:)')';
    %disp(transformGyro2(i,1) == inf);
%      qdot = 0.5*omega*transformGyro2(i-1,:)';
%      qdot = qdot*(timeLine(i) - timeLine(i-1))/1e9;
%      transformGyro2(i,:) = transformGyro2(i-1,:)+qdot';
%      end
  end

[r1 r2 r3] = quat2angle(transformGyro2);
global angleGyro2;
angleGyro2 = [r3  r2  r1];

angleGyro2(:,3) = pi+angleGyro2(:,3);

% index = find(angleGyro2(:,1)<0);
% angleGyro2(index,1) = 2*pi + angleGyro2(index,1);
% index = find(angleGyro2(:,2)<0);
% angleGyro2(index,2) = 2*pi + angleGyro2(index,2);
% index = find(angleGyro2(:,3)<0);
% angleGyro2(index,3) = 2*pi + angleGyro2(index,3);
clear rotationQ rotationAngle rotationAxis i magInitialOri
clear i;
clear newq;
clear qx;
clear qy;
clear qz
clear ax;
clear ay;
clear az;
clear situation;
clear gTotal;
clear gVectorCalAngle;
clear gVectorCalAxis;
clear initAccy;
clear initAccz;
clearvars height xyProjection i;
clear stableCount initAccx ;
clear stepQ wx wy wz roll pitch yaw q omega;
clear r1 r2 r3;
disp('attitude by gyro done');
toc;