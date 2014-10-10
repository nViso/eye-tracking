tic;
Q = diag([0.0001 0.0001 0.0001 ]);
R = diag([0.5 0.5 0.5]);
P = eye(3);
A = eye(3);

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

% angleCumu = zeros(size(gyro));
x = angleOri(80,:);
% angleCumu(1,:) = x;
angleEKF2 = zeros(size(gyro));
angleEKF2(1,:) = x;
% errors = [];
for i = 2:length(gyro)
      
    %A�ļ���
    wx = gyro(i,2); % pitch 
    wy = gyro(i,1); % roll
    wz = gyro(i,3); % yaw
    roll =  x(1);
    pitch = x(2);
    yaw   = x(3);
    omega = [1 sin(roll)*tan(pitch) cos(roll)*tan(pitch);
             0 cos(roll)            -sin(roll);
             0 sin(roll)/cos(pitch) cos(roll)/cos(pitch);];
%     omega(3,:) = [ 0 0 0];
    omega = (omega*[wy wx wz]')';
    x = x + omega*(timeLine(i) - timeLine(i-1))/1e9;
    
    
    %�����������
    P= A*P*A' +Q;
    
    %H�ļ���
    H = eye(3);  % ��Ϊeulerian angle ����������Ե�
    %����Kalman Gain
    e = H*P*H'+R;
    K = P*H'/e;
    
    error = angleOri(i,:) - x;
    error(3) = 0;
    error = error *addinGain(i);
    x = x + (K *error')';
    angleEKF2(i,:) = x;
    P = (eye(3)-K*H)*P;
    
end

angleEKF2(:,3) = angleEKF2(:,3) - mean(angleEKF2(800-400:800,3));
transformEKF2 = angle2quat(angleEKF2(:,3),angleEKF2(:,2),angleEKF2(:,1));

clear A H K P Q R angleCumu e error errors i pitch roll yaw wx wy wz x omega;
clear addinGain , count;
toc;
disp('Attitude EKF2 done');