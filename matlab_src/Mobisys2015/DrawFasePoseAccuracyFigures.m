clc;clear;close all;
load('FacePoseAccuracies.mat');

figureSize = [500 500 300 250 ];
axesPosition = [0.143333333333333 0.153333333333333 0.821666666666667 0.821666666666667];
colorbarPosition = [0.890172839506176 0.133333333333333 0.0468971193415638 0.811666666666667];


[ufacetobn(:,1), ufacetobn(:,2), ufacetobn(:,3)] = cart2sph(ufacetobn(:,1),ufacetobn(:,2),ufacetobn(:,3));
ufacetobn = ufacetobn - ones(length(ufacetobn),1)*mean(ufacetobn);
ufacetobn = rad2deg(ufacetobn);

figure1 = figure('Renderer','painters');
set(figure1,'Position',figureSize);
set(figure1,'Colormap',linspecer);
set(figure1,'DefaultAxesFontSize',13);

axes1 = axes('Parent',figure1,...
    'YTickLabel',{'0','10','20'},...
    'YTick',[-9.81599630471515 0.184003695284851 9.93966049489797],...
    'XTickLabel',{'0','30','60','90','120','150'},...
    'XTick',[-88.1909229208924 -58.1909229208924 -28.1909229208924 1.80907707910757 31.8090770791076 61.8090770791076],...
    'Position',axesPosition);
hold(axes1,'all');
box(axes1,'on');
xlim(axes1,[-88.1909229208924 63.8090770791076]);
ylim(axes1,[-9.81599630471515 9.93966049489797]);
scatter(ufacetobn(:,1),ufacetobn(:,2),35,(1:length(ufacetobn))/50,'filled');

hold off;

% Create xlabel
xlabel('Azimuth (\circ)');

% Create ylabel
ylabel('Elevation (\circ)');

save2pdf('FacePoseAccuracybn.pdf');


%%
[usensor(:,1), usensor(:,2), usensor(:,3)] = cart2sph(usensor(:,1),usensor(:,2),usensor(:,3));
usensor = usensor - ones(length(usensor),1)*mean(usensor);
usensor = rad2deg(usensor);
figure1 = figure;
set(figure1,'Position',figureSize);
set(figure1,'Colormap',linspecer);
set(figure1,'DefaultAxesFontSize',13);

axes1 = axes('Parent',figure1,...
    'ZTickLabel',{'0','0.10','0.15','0.2'},...
    'ZTick',[0.06 0.11 0.16 0.21],...
    'YTickLabel',{'0','5','10'},...
    'YTick',[-4.4055595822231 0.594440417776901 4.94440417776901],...
    'XTickLabel',{'0','10','20','30'},...
    'XTick',[-16.501014198783 -6.50101419878295 3.49898580121705 13.498985801217],...
    'Position',axesPosition);
hold(axes1,'all');
box(axes1,'on');
xlim(axes1,[-16.701014198783 18.4898580121705]);
ylim(axes1,[-4.4055595822231 4.94440417776901]);
scatter(usensor(:,1),usensor(:,2),35,(1:length(usensor))/50,'filled');
hold off;

% Create xlabel
xlabel('Azimuth (\circ)');

% Create ylabel
ylabel('Elevation (\circ)');

save2pdf('FacePoseAccuracySensor.pdf');


%%
[ufacenn(:,1), ufacenn(:,2), ufacenn(:,3)] = cart2sph(ufacenn(:,1),ufacenn(:,2),ufacenn(:,3));
ufacenn = ufacenn - ones(length(ufacenn),1)*mean(ufacenn);
ufacenn = rad2deg(ufacenn);
figure1 = figure;
set(figure1,'Position',figureSize);
set(figure1,'Colormap',linspecer);
set(figure1,'DefaultAxesFontSize',13);

axes1 = axes('Parent',figure1,...
    'ZTickLabel',{'0.42','0.425','0.43','0.435'},...
    'ZTick',[-0.47 -0.42 -0.37 -0.32],...
    'ZDir','reverse',...
    'YTickLabel',{'0','1','2'},...
    'YTick',[-4.9863226560015 0.0136773439985012 5.0136773439985],...
    'XTickLabel',{'0','1','2'},...
    'XTick',[-4.01616379310345 -0.0161637931034475 3.98383620689655],...
    'Position',axesPosition);
% view(axes1,[0 0]);
hold(axes1,'all');
box(axes1,'on');
set(axes1,'zDir','reverse');
xlim(axes1,[-4.11616379310345 5.58383620689655]);
ylim(axes1,[-4.9863226560015 5.33677343998501]);
scatter(ufacenn(:,1),ufacenn(:,2),35,(1:length(ufacenn))/50,'filled');

hold off;

% Create xlabel
xlabel('Azimuth (\circ)');

% Create ylabel
ylabel('Elevation (\circ)');

save2pdf('FacePoseAccuracyNN.pdf');

%%
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
    'XTickLabel',{'0','1','2','3','4','5'},...
    'XTick',[-8 -4 0 4 8 ],...
    'CLim',[1 100],...
    'Position',axesPosition);
xlim([-8 8]);
ylim([0 1]);
hold(axes1,'all');
box(axes1,'on');

[cdf1, xi1] = ksdensity(ufacenn(:,1),'function','cdf');
[cdf2, xi2] = ksdensity(ufacenn(:,2),'function','cdf');
[pdf1, xi1] = ksdensity(ufacenn(:,1),'function','pdf');
[pdf2, xi2] = ksdensity(ufacenn(:,2),'function','pdf');

colormap(axes1,[cool(32); linspecer(32);]);
zmin = min(cdf1(:));
zmax = max(cdf1(:));
% CDX ranges from 1 to 32.
cdx = min(32,round(31*(cdf1-zmin)/(zmax-zmin))+1);
% CDY ranges from 33 to 64.
cdy = cdx+32;

h1 = scatter(xi1,cdf1,70,1:length(xi1),'o','filled');
set(h1,'CData',cdx);
h2 = scatter(xi2,cdf2,70,1:length(xi2),'o','filled');
set(h2,'CData',cdy);
caxis([1 64])


t1index = 25; t2index = 55;
text(xi1(t1index),cdf1(t1index),' \leftarrow Azimuth','FontSize',14);
text(xi2(t2index),cdf2(t2index),' \leftarrow Elevation','FontSize',14);


% Create xlabel
xlabel('Angle (\circ)');

% Create ylabel
ylabel('Error CDF (%)');


save2pdf('FacePoseAccuracyCDF.pdf');