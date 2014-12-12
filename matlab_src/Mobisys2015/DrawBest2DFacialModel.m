addpath('../');
addpath('../fusing');
points = zeros(7,2,size(h,3));
for i = 1:size(h,3)
    hl = squeeze(h(:,:,i));
    mi = mdscale(hl,2);
    [d,z,t] = procrustes(xpoints,mi);
    mi = t.b*mi*t.T +t.c;
    points(:,:,i) = mi;
end
%%


%%
pts = [];
for i = 1:7
    pts = [ pts squeeze(points(i,:,:)) ];
end
% pts =flipud(pts);
dist = kde(pts, [0.05 0.05]',[],'E' );

[r c] = meshgrid(-0.65:0.01:0.65,-0.2:0.01:1);
v = evaluate(dist,[r(:) c(:)]');
v = reshape(v,size(r));

%%

figureSize = [500 500 450 250 ];
axesPosition = [0.0356666666666667 0.2033333333333 0.838333333333335 0.861666666666667];
colorbarPosition = [0.890172839506176 0.133333333333333 0.0468971193415638 0.811666666666667];

figure1 = figure;
set(figure1,'Position',figureSize);
set(figure1,'Colormap',linspecer);
set(figure1,'DefaultAxesFontSize',13);
axes1 = axes('Parent',figure1,...
    'YTickLabel',{'0','25','50','75','100','125'},...
    'YTick',[-0.1 0.1 0.3 0.5 0.7 0.9],...
    'YDir','reverse',...
    'XTickLabel',{'0','25','50','75','100'},...
    'XTick',[-0.6 -0.3 0 0.3 0.6],...
    'ZTick',[],...
    'Position',axesPosition);
xlim(axes1,[-0.6 0.6]);
ylim(axes1,[-0.1 0.9]);
zlim(axes1,[0 30]);
view(axes1,[0 73]);
grid on;

% Create xlabel
xlabel('Horizontal (mm)');

% Create ylabel
ylabel('Vertical (mm)');

set(axes1,'yDir','reverse');
% set(axes1,'Visible','off');
hold(axes1,'all');
surfc(r,c,v,'FaceColor','interp',...
    'EdgeColor','none');
alpha('color');
scatter(pts(1,:),pts(2,:),100,[44 100 170]/256,'+');
% coloredScatter(pts');

plot(xpoints([1 2],1),xpoints([1 2],2),'Color',[181 56 74]/256,'LineWidth',2);
plot(xpoints([1 3],1),xpoints([1 3],2),'Color',[181 56 74]/256,'LineWidth',2);
plot(xpoints([2 4],1),xpoints([2 4],2),'Color',[181 56 74]/256,'LineWidth',2);
plot(xpoints([1 6],1),xpoints([1 6],2),'Color',[181 56 74]/256,'LineWidth',2);
plot(xpoints([2 5],1),xpoints([2 5],2),'Color',[181 56 74]/256,'LineWidth',2);
plot(xpoints([5 6],1),xpoints([5 6],2),'Color',[181 56 74]/256,'LineWidth',2);
plot(xpoints([5 7],1),xpoints([5 7],2),'Color',[181 56 74]/256,'LineWidth',2);
plot(xpoints([6 7],1),xpoints([6 7],2),'Color',[181 56 74]/256,'LineWidth',2);
scatter(xpoints(:,1),xpoints(:,2),200,[181 56 74]/256,'fill');
hold off;
opengl('OpenGLLineSmoothingBug',1);
save2eps('Best2DFacialModel.eps',gcf,'opengl');
path1 = getenv('PATH');
path1 = [path1 ':/usr/local/bin' ':/usr/texbin'];
setenv('PATH',path1);
system('epspdf ./Best2DFacialModel.eps');
system('rm ./Best2DFacialModel.eps');