tic
angle = zeros(size(gyro)); %��gyro�ĽǶ�ֱ�ӻ���
stepIncremental =zeros(size(gyro)); %ÿһʱ�̵�ʵ�ʽǶ�����
transformGyro = zeros(length(gyro),4) ; %����ϵ--->����ϵ��Quaternion
gVectorByGyro =zeros(size(gyro));   

%���ȼ���ÿһʱ�̵�ʵ������
for i = 2:length(gyro)
    %�����ʱ�̱��ж�Ϊ���˶����������֮
   if  motionState(i)>0.5
        %���������stepIncremental������ֱ֮�ӻ��ֵõ�angle
        stepIncremental(i,2) = (gyro(i-1,1)+gyro(i,1))*(timeLine(i) - timeLine(i-1))/1e9/2 ;
        angle(i,2) = angle(i-1,1) + stepIncremental(i,1);
        stepIncremental(i,1) =- (gyro(i-1,2)+gyro(i,2))*(timeLine(i) - timeLine(i-1))/1e9/2;
        angle(i,1) = angle(i-1,2) + stepIncremental(i,2);
        stepIncremental(i,3) =- (gyro(i-1,3)+gyro(i,3))*(timeLine(i) - timeLine(i-1))/1e9/2;
        angle(i,3) = angle(i-1,3) + stepIncremental(i,3);
   %��֮ά����һʱ��״̬
   else
       angle(i,:) = angle(i-1,:);
       stepIncremental(i,:) = [0 0 0 ];
    end
end

angle = rad2deg(angle);

%model������������
axisXgyro = zeros(size(gyro));
axisYgyro = zeros(size(gyro));
axisZgyro = zeros(size(gyro));
%ˮƽ���Ϸ����ͶӰ
horizonHeading = zeros(length(gyro),2);
%��ֵ

motionIndex = find(motionState>0.5);
motionStart = 1;
if ~isempty(motionIndex)
    motionStart = motionIndex(1);
    for i =1:motionStart
        initAccx = mean(gravityByAcc(i,1));
        initAccy = mean(gravityByAcc(i,2));
        initAccz = mean(gravityByAcc(i,3));
        
        % gVectorByGyro����ָ��gyro����Ļ���ϵ�е���������
        gVectorByGyro(i,:) = [initAccx initAccy initAccz];
        gVectorByGyro(i,:) = gVectorByGyro(i,:)/norm(gVectorByGyro(i,:));

        %��gyro��ģ��У������ʵ�ʻ�����бһ��
        gVectorCalAxis = cross([ 0 0 1],gVectorByGyro(i,:));
        gVectorCalAxis = gVectorCalAxis/norm(gVectorCalAxis); %��ҲҪnormalize������ԣ�
        gVectorCalAngle = acos(dot([0 0 1],gVectorByGyro(i,:)));
        %���У����ת����ĽǶȺ��ᣬ��ת����quaternions

        gVectorCalQuat = [cos(gVectorCalAngle/2),sin(gVectorCalAngle/2)*gVectorCalAxis];
        gVectorCalQuat = quatnormalize(gVectorCalQuat);
        
        %��������������ת�������ʵ����̬һ��
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
   
   %euler����ת����˳��ģ�һ���ZYX��ת�������ŵģ�
   %���Դ˴�ʹ��Big Angle first������˳��
   
   if ax>=ay && ay >=az situation = 5; end
   if ax>=az && az >=ay situation = 4; end
   if ay>=ax && ax >=az situation = 3; end
   if ay>=az && az>=ax  situation = 2; end
   if az>=ax && ax>=ay  situation = 1; end
   if az>=ay && ay>=ax  situation = 0; end
   
   %ax,ay,az��������������ǰһʱ����̬�Ļ����Ͻ�һ����ת��
   
   qy = [cos(ay/2),sin(ay/2)*axisYgyro(i-1,:)];
   qz = [cos(az/2),sin(az/2)*axisZgyro(i-1,:)];
   qx = [-cos(ax/2),sin(ax/2)*axisXgyro(i-1,:)];
   
   
   %��������������������
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
   %��̬ת��quaternionҲ���и���
   transformGyro(i,:) = quatmultiply(transformGyro(i-1,:),newq);
   %��������������̬����
   axisXgyro(i,:)  = quatrotate(newq,axisXgyro(i-1,:));
   axisYgyro(i,:)  = quatrotate(newq,axisYgyro(i-1,:));
   axisZgyro(i,:)  = quatrotate(newq,axisZgyro(i-1,:));
%    axisXgyro(i,:)  = quatrotate(transformGyro(i,:),axisXgyro(1,:));
%    axisYgyro(i,:)  = quatrotate(transformGyro(i,:),axisYgyro(1,:));
%    axisZgyro(i,:)  = quatrotate(transformGyro(i,:),axisZgyro(1,:));
   %����������Ҳ���и���,����ת�ĽǶ�ȴ�෴
    gVectorByGyro(i,:)    = quatrotate(quatconj(newq),gVectorByGyro(i-1,:));
   
    %��⵱ǰ��̬��ˮƽ����
    %axisYgyro=height + xyProjection;
    %��� xyProjection = axisYgyro - height;
    height = dot(axisYgyro(i,:),[0 0 1]);
    height = height * [0 0 1];
    xyProjection = axisYgyro(i,:) - height;
    xyProjection  = xyProjection/norm(xyProjection);
    horizonHeading(i,:) = xyProjection(1:2);
    else
          %�����ǰ�˶�״̬Ϊ����ֹ������һ�ж�ά����һ��̬
          axisXgyro(i,:) = axisXgyro(i-1,:);
          axisYgyro(i,:) = axisYgyro(i-1,:);  
          axisZgyro(i,:) = axisZgyro(i-1,:);  
          gVectorByGyro(i,:) = gVectorByGyro(i-1,:);
          transformGyro(i,:) = transformGyro(i-1,:);
          horizonHeading(i,:) = horizonHeading(i-1,:);
    end
   %���뾲̬�����߼���
   %���Ȳ���50���㣬ͨ����50���������ǰ��
   
   
   if motionState(i) <0.5
       stableCount = stableCount+1;
       
       else stableCount = 0;
   end
   
   %��һ�δ��������⣬������count>30000��ʱ����
   if stableCount>300000
       %�ӹ�ȥ50��gravity�еõ�gVector
       initAccx = mean(gravityByAcc(i-190:i,1));
       initAccy = mean(gravityByAcc(i-190:i,2));
       initAccz = mean(gravityByAcc(i-190:i,3));
       gVector = [initAccx initAccy initAccz];
       gTotal = norm(gVector);
       gVector = gVector/norm(gVector);
       
       %�ٸ���gVector��horizonHeading���¹�����ǰ��axisGyro
       %�ȹ���һ������gravity���Ƶ�bϵ���ٰ�����horizonHeading������ת
       gVectorCalAxis = cross(gVectorByGyro(i-1,:),gVector);
       gVectorCalAxis = gVectorCalAxis/norm(gVectorCalAxis); %��ҲҪnormalize������ԣ�
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