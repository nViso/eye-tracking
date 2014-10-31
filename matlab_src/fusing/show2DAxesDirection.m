function show2DAxesDirection(quat)

meshgrid([1 0 0],1:length(quat));
c = quatrotate(quat,ans);
figure; plot(c);
% xlabel('x');
% ylabel('y');
% zlabel('z');
% scatter3(c(:,1),c(:,2),c(:,3),20,[1:length(c)]','filled');
meshgrid([0 1 0],1:length(quat));
c = quatrotate(quat,ans);
figure; plot(c);
% xlabel('x');
% ylabel('y');
% zlabel('z');
% scatter3(c(:,1),c(:,2),c(:,3),20,[1:length(c)]','filled');
meshgrid([0 0 1],1:length(quat));
c = quatrotate(quat,ans);
figure; plot(c);
% xlabel('x');
% ylabel('y');
% zlabel('z');
% scatter3(c(:,1),c(:,2),c(:,3),20,[1:length(c)]','filled');