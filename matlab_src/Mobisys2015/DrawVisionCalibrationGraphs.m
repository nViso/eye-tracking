close all;
addpath('../');
p = 10;
w = fa1(1:p:end);
x = fa2(1:p:end);
y = fa3(1:p:end);
z = g2(1:p:end);
z = z/20;

step = 0.01;
wrange(1) = min(w); wrange(2) = max(w);
xrange(1) = min(x); xrange(2) = max(x);
yrange(1) = min(y); yrange(2) = max(y);


[xx yy] = meshgrid(xrange(1):step:xrange(2), yrange(1):step:yrange(2));
[xq yq vq] = griddata(x,y,z,xx,yy);


%%
figureSize = [500 500 300 250 ];
axesPosition = [0.186666666666667 0.103333333333333 0.738333333333335 0.861666666666667];
colorbarPosition = [0.890172839506176 0.133333333333333 0.0468971193415638 0.811666666666667];

figure1 = figure;
set(figure1,'Position',figureSize);
set(figure1,'Colormap',linspecer);
set(figure1,'DefaultAxesFontSize',13);
% Create axes
axes1 = axes('Parent',figure1,...
    'ZTickLabel',{'-0.05','0','-0.05'},...
    'ZTick',[-0.08  -0.015  0.05],...
    'YTickLabel',{'-0.4','0','-0.4'},...
    'YTick',[-4.2  -3.2  -2.2],...
    'XTickLabel',{'-0.4','0','0.4'},...
    'XTick',[-0.9  -0.1  0.7],...
    'Position',axesPosition);

xlim(axes1,[-0.9 0.7]);
ylim(axes1,[-4.2 -2.2]);
zlim(axes1,[-0.08 0.05]);
view(axes1,[-45 26]);
grid(axes1,'on');
hold(axes1,'all');
% Create xlabel
xlabel('Pitch (rad)');

% Create ylabel
ylabel('Roll (rad)');

% Create zlabel
zlabel('Error in Pitch (rad)');


s = surfc(xq,yq,vq,'Parent',axes1,'FaceColor','interp',...
    'EdgeColor','none');
alpha(s,0.6);

scatter3(x,y,z,70,z+0.01,'filled');
opengl('OpenGLLineSmoothingBug',1);

save2pdf('VisionAngleError.eps',gcf,'opengl');
path1 = getenv('PATH');
path1 = [path1 ':/usr/local/bin' ':/usr/texbin'];
setenv('PATH',path1);
system('epspdf ./VisionAngleError.eps');
system('rm ./VisionAngleError.eps');
%%
[xx yy] = ndgrid(xrange(1):step:xrange(2), yrange(1):step:yrange(2));
points = [xx(:) yy(:)];
density = evaluate(kde([x y]',[0.1 0.1]'),points')';
density = reshape(density,size(xx));


figureSize = [500 500 300 250 ];
axesPosition = [0.186666666666667 0.103333333333333 0.738333333333335 0.861666666666667];

figure1 = figure;
set(figure1,'Position',figureSize);
set(figure1,'Colormap',linspecer);
set(figure1,'DefaultAxesFontSize',13);
% Create axes
axes1 = axes('Parent',figure1,...
    'ZTick',[0 6],...
    'ZTickLabel',{'',''},...
    'YTickLabel',{'-0.4','0','-0.4'},...
    'YTick',[-4.2  -3.2  -2.2],...
    'XTickLabel',{'-0.4','0','0.4'},...
    'XTick',[-0.9  -0.1  0.7],...
    'Position',axesPosition);

xlim(axes1,[-0.9 0.7]);
ylim(axes1,[-4.2 -2.2]);
zlim(axes1,[0 6]);
view(axes1,[-45 26]);
grid(axes1,'on');
hold(axes1,'all');
% Create xlabel
xlabel('Pitch (rad)');

% Create ylabel
ylabel('Roll (rad)');

% Create zlabel
zlabel('angle density');


s = surfc(xx,yy,density,'Parent',axes1,'FaceColor','interp',...
    'EdgeColor','none');
alpha(s,'color');

scatter(x,y,70,z*200,'filled');
opengl('OpenGLLineSmoothingBug',1);
set(figure1,'Position',figureSize);
save2pdf('VisionAnglePDF.eps',gcf,'opengl');
path1 = getenv('PATH');
path1 = [path1 ':/usr/local/bin' ':/usr/texbin'];
setenv('PATH',path1);
system('epspdf ./VisionAnglePDF.eps');
system('rm ./VisionAnglePDF.eps');