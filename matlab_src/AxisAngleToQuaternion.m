function quat = AxisAngleToQuaternion(axisAngle)

angles = sqrt(sum(abs(axisAngle).^2,2));
axis = normr(axisAngle);
halfAnglesSine = sin(angles/2);
halfAngleCosine = cos(angles/2);
quat = [halfAngleCosine meshgrid(halfAnglesSine,1:3)'.*axis];

quat = quatnormalize(quat);