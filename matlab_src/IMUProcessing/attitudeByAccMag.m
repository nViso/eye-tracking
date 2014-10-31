tic;


accfilted = filtfilt(ones(1,85)/85,1,acc);
magfilted = filtfilt(ones(1,95)/95,1,mag);

ori2 = zeros(length(acc),3);
ref = [ -1 0 0];
for i = 1:length(acc)
    
    earth = accfilted(i,1:3);
    o1 = cross(earth,ref);
    o2 = cross(earth,o1);
    m = magfilted(i,1:3);
    %m = [30 0 -30];
    o1l = dot(o1,m)*norm(o2);
    o2l = dot(o2,m)*norm(o1);
    y = atan2(o2l,o1l);
    if(y<0) 
        y = 2*pi + y;
    end
    ori2(i,3) = y;
    %pitch
    ori2(i,2) = asin(earth(2)/norm(earth));
    
    %roll
    ori2(i,1) =asin(-earth(1)/norm(earth));
end

angleOri = ori2;
clear ori2;

transformOri = angle2quat(angleOri(:,3),angleOri(:,2),angleOri(:,1));

bacc = acc;
clear a1* a2* a3* err;
acc = bacc;
clear bacc;
clear e h m a qs qw qx qy qz em am hm mm anorm hnorm;
clear o1 o1l o2 o2l ref  k i y p r0* r1* r2* ;
clear yaw pitch roll norms height;
clear height xyProj*
clear q q? w? qnorm;
 clear *Part;
clear earth;
clear r3 r2 r1;
clear accfilted magfilted; 
disp('Attitude By Acc/mag Done.');
toc;