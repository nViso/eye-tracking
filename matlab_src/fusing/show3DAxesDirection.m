function [xvec yvec zvec] = show3DAxesDirection(quat)

xvec = quatrotate(quat,[1 0 0]);
yvec = quatrotate(quat,[0 1 0]);
zvec = quatrotate(quat,[0 0 1]);

xvec = normr(xvec);
yvec = normr(yvec);
zvec = normr(zvec);
figure;
grid on;
hold on;
coloredScatter(xvec);
coloredScatter(yvec);
coloredScatter(zvec);
text(median(xvec(:,1)),median(xvec(:,2)),median(xvec(:,3)),'    \leftarrow      X');
text(median(yvec(:,1)),median(yvec(:,2)),median(yvec(:,3)),'    \leftarrow      Y');
text(median(zvec(:,1)),median(zvec(:,2)),median(zvec(:,3)),'    \leftarrow      Z');
hold off;
set(gca,'ZDir','reverse')
xlim([-1 1]);
ylim([-1 1]);
zlim([-1 1]);
xlabel('North (X)');
ylabel('East (Y)');
zlabel('Down (Z)');

if nargout < 1
    xvec = [];
    yvec = [];
    zvec = [];
end