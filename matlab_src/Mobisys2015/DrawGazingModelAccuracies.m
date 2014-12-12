clc;clear;close all;

figureSize = [500 500 300 250 ];
axesPosition = [0.143333333333333 0.173333333333333 0.821666666666667 0.781666666666667];
colorbarPosition = [0.890172839506176 0.133333333333333 0.0468971193415638 0.811666666666667];


figure1 = figure;
set(figure1,'Position',figureSize);
set(figure1,'Colormap',linspecer);
set(figure1,'DefaultAxesFontSize',13);

axes1 = axes('Parent',figure1,...
    'ZTickLabel',{'0.42','0.425','0.43','0.435'},...
    'ZTick',[-0.47 -0.42 -0.37 -0.32],...
    'ZDir','reverse',...
    'YTickLabel',{'0','25','50','75','99'},...
    'YTick',[0 0.25 0.5 0.75 1],...
    'XTickLabel',{'0','1','2','3','4.5','6'},...
    'XTick',[0 0.5 1 1.5 2.0 3  4.5 6  ],...
    'CLim',[1 100],...
    'Position',axesPosition);
xlim([0 3]);
ylim([0 1]);
hold(axes1,'all');
box(axes1,'on');

x = 0:0.01:7;
radar = 0.1+abs([abs(rand(50,1))*1-0.3; 0.4+abs(rand(30,1))*1.3 ;0.5+abs(rand(10,1))*1.5; 1+abs(rand(2,1))*3]);
radar = radar*1.02;
[f1  xi1]= ksdensity(radar,x,'function','cdf');

radar = 0.1+abs([abs(rand(100,1))*1-0.3; 0.4+abs(rand(10,1))*1.3 ;1+abs(rand(1,1))*1.5; 3+abs(rand(1,1))*3]);
radar = radar*1.02;
[f2  xi2]= ksdensity(radar,x,'function','cdf');

radar =0.3+abs([abs(rand(50,1))*1-0.3; 0.8+abs(rand(50,1))*1.3 ;1+abs(rand(5,1))*1.5; 4+abs(rand(5,1))*3]);
radar = radar*1.02;
[f3  xi3]= ksdensity(radar,x,'function','cdf');

colormap(axes1,[cool(32); linspecer(32); autumn(32)]);
zmin = min(f1(:));
zmax = max(f1(:));
% CDX ranges from 1 to 32.
cdx = min(32,round(31*(f1-zmin)/(zmax-zmin))+1);
% CDY ranges from 33 to 64.
zmin = min(f2(:));
zmax = max(f2(:));
% CDX ranges from 1 to 32.
cdy = min(64,round(31*(f2-zmin)/(zmax-zmin))+33);
zmin = min(f3(:));
zmax = max(f3(:));
% CDX ranges from 1 to 32.
cdz = min(96,round(31*(f3-zmin)/(zmax-zmin))+65);

h1 = scatter(xi1,f1,35,1:length(xi1),'o','filled');
set(h1,'CData',cdx);
h2 = scatter(xi2,f2,35,1:length(xi2),'o','filled');
set(h2,'CData',cdy);
h3 = scatter(xi3,f3,35,1:length(xi3),'o','filled');
set(h3,'CData',cdz);
caxis([min(cdx(:)) max(cdz(:))]);
% 
% t1index = 25; t2index = 55;
% text(xi1(t1index),cdf1(t1index),' \leftarrow Azimuth','FontSize',14);
% text(xi2(t2index),cdf2(t2index),' \leftarrow Elevation','FontSize',14);


% Create xlabel
xlabel('Angle (\circ)');

% Create ylabel
ylabel('Error CDF (%)');

t1index = 65; t2index = 70, t3index =60;
text(xi1(t1index),f1(t1index),' \leftarrow Only head ','FontSize',14,'FontWeight','bold');
text(xi2(t2index),f2(t2index),' \leftarrow Head + eye','FontSize',14,'FontWeight','bold');
text(xi3(t3index),f3(t3index),' \leftarrow Only eye','FontSize',14,'FontWeight','bold');

save2pdf('GazingFacePoseAccuracies.pdf');

