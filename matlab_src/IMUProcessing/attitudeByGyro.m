tic
angle = zeros(size(gyro)); %对gyro的角度直接积分
stepIncremental =zeros(size(gyro)); %每一时刻的实际角度增量
transformGyro = zeros(length(gyro),4) ; %机器系--->导航系的Quaternion
gVectorByGyro =zeros(size(gyro));   

%首先计算每一时刻的实际增量
for i = 2:length(gyro)
    %如果此时刻被判定为“运动”，则积分之
   if  motionState(i)>0.5
        %计算角增量stepIncremental，并对之直接积分得到angle
        stepIncremental(i,2) = (gyro(i-1,1)+gyro(i,1))*(timeLine(i) - timeLine(i-1))/1e9/2 ;
        angle(i,2) = angle(i-1,1) + stepIncremental(i,1);
        stepIncremental(i,1) =- (gyro(i-1,2)+gyro(i,2))*(timeLine(i) - timeLine(i-1))/1e9/2;
        angle(i,1) = angle(i-1,2) + stepIncremental(i,2);
        stepIncremental(i,3) =- (gyro(i-1,3)+gyro(i,3))*(timeLine(i) - timeLine(i-1))/1e9/2;
        angle(i,3) = angle(i-1,3) + stepIncremental(i,3);
   %反之维持上一时刻状态
   else
       angle(i,:) = angle(i-1,:);
       stepIncremental(i,:) = [0 0 0 ];
    end
end

angle = rad2deg(angle);

%model出三个轴向量
axisXgyro = zeros(size(gyro));
axisYgyro = zeros(size(gyro));
axisZgyro = zeros(size(gyro));
%水平面上方向的投影
horizonHeading = zeros(length(gyro),2);
%初值

motionIndex = find(motionState>0.5);
motionStart = 1;
if ~isempty(motionIndex)
    motionStart = motionIndex(1);
    for i =1:motionStart
        initAccx = mean(gravityByAcc(i,1));
        initAccy = mean(gravityByAcc(i,2));
        initAccz = mean(gravityByAcc(i,3));
        
        % gVectorByGyro，是指在gyro解算的机体系中的重力向量
        gVectorByGyro(i,:) = [initAccx initAccy initAccz];
        gVectorByGyro(i,:) = gVectorByGyro(i,:)/norm(gVectorByGyro(i,:));

        %将gyro的模型校正，与实际机器倾斜一致
        gVectorCalAxis = cross([ 0 0 1],gVectorByGyro(i,:));
        gVectorCalAxis = gVectorCalAxis/norm(gVectorCalAxis); %轴也要normalize才能算对！
        gVectorCalAngle = acos(dot([0 0 1],gVectorByGyro(i,:)));
        %算出校正旋转所需的角度和轴，并转换成quaternions

        gVectorCalQuat = [cos(gVectorCalAngle/2),sin(gVectorCalAngle/2)*gVectorCalAxis];
        gVectorCalQuat = quatnormalize(gVectorCalQuat);
        
        %将三个轴向量旋转至与机器实际姿态一致
        axisXgyro(i,:) = quatrotate(gVectorCalQuat,[1 0 0]);
        axisYgyro(i,:) = quatrotate(gVectorCalQuat,[0 1 0]);
        axisZgyro(i,:) = quatrotate(gVectorCalQuat,[0 0 1]);
        transformGyro(i,:) = gVectorCalQuat;
        
        clear gVectorCalQuat;
    end
end
stableCount = 0;
for i = motionStart+1 : length(gyro)
    if  motionState(i)  >0.5
   
   ax = stepIncremental(i,1);
   ay = stepIncremental(i,2);
   az = stepIncremental(i,3);
   situation = 0;
   
   %euler角旋转是有顺序的，一般的ZYX旋转不是最优的，
   %所以此处使用Big Angle first来决定顺序
   
   if ax>=ay && ay >=az situation = 5; end
   if ax>=az && az >=ay situation = 4; end
   if ay>=ax && ax >=az situation = 3; end
   if ay>=az && az>=ax  situation = 2; end
   if az>=ax && ax>=ay  situation = 1; end
   if az>=ay && ay>=ax  situation = 0; end
   
   %ax,ay,az都是增量，是在前一时刻姿态的基础上进一步的转动
   
   qy = [cos(ay/2),sin(ay/2)*axisYgyro(i-1,:)];
   qz = [cos(az/2),sin(az/2)*axisZgyro(i-1,:)];
   qx = [-cos(ax/2),sin(ax/2)*axisXgyro(i-1,:)];
   
   
   %将这三个向量连接起来
   newq = [ 0 0 0 0 ];
   switch (situation)
       case 0 
           newq = quatmultiply(qz,quatmultiply(qy,qx)); 
       case 1 
           newq = quatmultiply(qy,quatmultiply(qz,qx));
       case 2 
           newq = quatmultiply(qz,quatmultiply(qx,qy));
       case 3 
           newq = quatmultiply(qx,quatmultiply(qz,qy));
       case 4 
           newq = quatmultiply(qy,quatmultiply(qx,qz));
       case 5 
           newq = quatmultiply(qx,quatmultiply(qy,qz));
   end
   %姿态转换quaternion也进行更新
   transformGyro(i,:) = quatmultiply(transformGyro(i-1,:),newq);
   %对三个轴向量姿态更新
   axisXgyro(i,:)  = quatrotate(newq,axisXgyro(i-1,:));
   axisYgyro(i,:)  = quatrotate(newq,axisYgyro(i-1,:));
   axisZgyro(i,:)  = quatrotate(newq,axisZgyro(i-1,:));
%    axisXgyro(i,:)  = quatrotate(transformGyro(i,:),axisXgyro(1,:));
%    axisYgyro(i,:)  = quatrotate(transformGyro(i,:),axisYgyro(1,:));
%    axisZgyro(i,:)  = quatrotate(transformGyro(i,:),axisZgyro(1,:));
   %对重力向量也进行更新,但旋转的角度却相反
    gVectorByGyro(i,:)    = quatrotate(quatconj(newq),gVectorByGyro(i-1,:));
   
    %求解当前姿态的水平方向
    %axisYgyro=height + xyProjection;
    %因此 xyProjection = axisYgyro - height;
    height = dot(axisYgyro(i,:),[0 0 1]);
    height = height * [0 0 1];
    xyProjection = axisYgyro(i,:) - height;
    xyProjection  = xyProjection/norm(xyProjection);
    horizonHeading(i,:) = xyProjection(1:2);
    else
          %如果当前运动状态为“静止”，则一切都维持上一姿态
          axisXgyro(i,:) = axisXgyro(i-1,:);
          axisYgyro(i,:) = axisYgyro(i-1,:);  
          axisZgyro(i,:) = axisZgyro(i-1,:);  
          gVectorByGyro(i,:) = gVectorByGyro(i-1,:);
          transformGyro(i,:) = transformGyro(i-1,:);
          horizonHeading(i,:) = horizonHeading(i-1,:);
    end
   %加入静态修正逻辑。
   %首先采样50个点，通过这50个点求出当前的
   
   
   if motionState(i) <0.5
       stableCount = stableCount+1;
       
       else stableCount = 0;
   end
   
   %这一段代码有问题，所以设count>30000暂时不用
   if stableCount>300000
       %从过去50个gravity中得到gVector
       initAccx = mean(gravityByAcc(i-190:i,1));
       initAccy = mean(gravityByAcc(i-190:i,2));
       initAccz = mean(gravityByAcc(i-190:i,3));
       gVector = [initAccx initAccy initAccz];
       gTotal = norm(gVector);
       gVector = gVector/norm(gVector);
       
       %再根据gVector和horizonHeading重新构建当前的axisGyro
       %先构建一个符合gravity走势的b系，再把它以horizonHeading方向旋转
       gVectorCalAxis = cross(gVectorByGyro(i-1,:),gVector);
       gVectorCalAxis = gVectorCalAxis/norm(gVectorCalAxis); %轴也要normalize才能算对！
       gVectorCalAngle = -acos(dot(gVectorByGyro(i-1,:),gVector));
       gVectorCalQuat = [cos(gVectorCalAngle/2),sin(gVectorCalAngle/2)*gVectorCalAxis];
       gVectorCalQuat = quatnormalize(gVectorCalQuat);
       
       gVectorByGyro(i,:) = quatrotate(gVectorCalQuat,gVectorByGyro(i-1,:));
          axisXgyro(i,:)  = quatrotate(gVectorCalQuat,[1 0 0]);
          axisYgyro(i,:)  = quatrotate(gVectorCalQuat,[0 1 0]);
          axisZgyro(i,:)  = quatrotate(gVectorCalQuat,[0 0 1]);
          transformGyro(i,:) = gVectorCalQuat;
          disp([ 'calibrated @' num2str(i)]);
       stableCount = 0;
       
       height = dot(axisYgyro(i,:),[0 0 1]);
       height = height * [0 0 1];
       xyProjection = axisYgyro(i,:) - height;
       xyProjection  = xyProjection/norm(xyProjection);
       horizonHeading(i,:) = xyProjection(1:2);
   end
       
end


[r1 r2 r3] = quat2angle(transformGyro);
angleGyro = [r3 r2 r1];

clear rotationQ rotationAngle rotationAxis i magInitialOri
clear i;
clear newq;
clear qx;
clear qy;
clear qz
% clear angle;
clear ax;
clear ay;
clear az;
clear situation;
% clear stepIncremental;
clear gTotal;
clear gVectorCalAngle;
clear gVectorCalAxis;
clear initAccy;
clear initAccz;
clearvars height xyProjection i;
clear stableCount initAccx ;
clear stepQ;
toc
disp('attitude calculation by gyro done');