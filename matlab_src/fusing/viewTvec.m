q = visionData.quaternion;
t = visionData.tvec;
s = 20*(1./visionData.projectionErrorRMS);

c = visionData.tc_vec;

figure1= figure;
axes1= axes('Parent',figure1,'DataAspectRatio',[1 1 1]);
grid(axes1,'on');
hold(axes1,'all');
scatter3(t(:,1),t(:,2),t(:,3),s,[1:length(t)]','filled');
figure2 = figure;
axes2 = axes('Parent',figure2,'DataAspectRatio',[1 1 1]);
grid(axes2,'on');
hold(axes2,'all');
scatter3(c(:,1),c(:,2),c(:,3),s,[1:length(c)]','filled');