
tic;

Q = diag([0.01 0.01 0.01 0.01]);
R = diag([0.01 0.01 0.01]);
P = eye(4);
q = transformOri(80,:)';
% qgyro = q;
transformEKF = zeros(length(gyro),4);
% qgyros = zeros(length(gyro),4);
transformEKF(1,:) = [1 0 0 0];

addinGain = ones(length(gyro),1);
count = 0;
for i = 1:length(gyro)
    if motionState(i) <0.5
        count = 0;
    else
         count  = count +1;
         addinGain(i) = (count/1500);
    end

end
addinGain(find(addinGain<1)) = 0;
addinGain(find(addinGain>1)) = 1;

for i = 2:length(gyro)
    if motionState(i) <0.5
        R = diag([0.1 0.1 0.1]);
    else
        R = diag([10 10 10]);
    end
    %A�ļ���
    wx = gyro(i,2);
    wy = gyro(i,1);
    wz = -gyro(i,3);
    A =     [0  -wx -wy -wz;
             wx   0  wz -wy;
             wy -wz   0  wx;
             wz  wy -wx   0;];
    A = A *0.5 *(timeLine(i) - timeLine(i-1))/1e9;
    A = A+eye(4);
%     qgyro = A*qgyro;
    %����qest
    q = A*q;
    q = (quatnormalize(q'))';
    %�����������
    P= A*P*A' +Q;
    
    %H�ļ���
    H = jacobianMatrix(q');
    %����Kalman Gain
    e = H*P*H'+R;
    K = P*H'/e;
    [r1 r2 r3] = quat2angle(q');
    angleQ = [r3 r2 r1];
    error = angleOri(i,:) - angleQ;
    %��3Ϊ0,����ȥ�����?���Ӱ��
    %��error(3) = 0;
    error = error *addinGain(i);
    %error��roll pitch yaw��˳��
    addin = K*(error');
    q = q+addin;
    q = (quatnormalize(q'))';
    P = (eye(4)-K*H)*P;
    
    transformEKF(i,:) = q';
%     qgyros(i,:) = qgyro';
end


[r1 r2 r3] = quat2angle(transformEKF);
angleEKF = [ r3 r2 r1];

% [r1 r2 r3] = quat2angle(qgyros);
% angleGyro = [ r3 r2 r1];
disp('attitude by EKF Done.');
toc;

clear r1 r2 r3  Q P q addin K H error angleQ wx wy wz  count A R e i;