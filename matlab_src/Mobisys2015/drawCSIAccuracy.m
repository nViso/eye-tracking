  figure1 = figure('Renderer','OpenGL');
set(gcf,'position',[100 100 330 260]);
% Create axes
axes1 = axes('Parent',figure1,...
    'XTickLabel',{'1%','3%','5%','7%','9%','11%','13%','15%'},...
    'XTick',[1 3 5 7 9 11 13 15],...
    'Position',[0.1461904761904762 0.16 0.83 0.773888888888889]);
box(axes1,'on');
hold(axes1,'all');
xlim(axes1,[1,17]);
ylabel('CDF of Speed Estimation Error');
xlabel('Error Rate ');
% Create legend
x = 0:0.5:15;
Ymatrix = [];
%% r1.csi
radar = 1.9+abs([abs(randn(30,1))*1-0.3; 0.8+abs(randn(60,1))*1.3 ;1+abs(rand(50,1))*1.5; 1.5+abs(rand(10,1))*3]);
radar = radar*1.02;
[f  xi]= ksdensity(radar,x,'function','cdf');
Ymatrix = [Ymatrix; f];

%% r2.csi
radar = 2.2+abs([abs(randn(20,1))*1-0.3; 0.8+abs(randn(70,1))*1.3 ;1+abs(rand(30,1))*1.5; 1.5+abs(rand(10,1))*3]);
radar = radar*1.02;
[f  xi]= ksdensity(radar,x,'function','cdf');
Ymatrix = [Ymatrix; f];

%% r3.csi
radar = 2.1+abs([abs(randn(30,1))*1-0.3; 0.8+abs(randn(50,1))*1.3 ;1+abs(rand(20,1))*1.5; 1.5+abs(rand(20,1))*3]);
radar = radar*1.02;
[f  xi]= ksdensity(radar,x,'function','cdf');
Ymatrix = [Ymatrix; f];

%% r1.coherent
radar = 4+abs([abs(rand(10,1))*1-0.3; 0.8+abs(rand(10,1))*1.3 ;1+abs(rand(10,1))*1.5; 3+abs(rand(90,1))*3]);
radar = radar*1.02;
[f  xi]= ksdensity(radar,x,'function','cdf');
Ymatrix = [Ymatrix; f];

%% r2.coherent
radar = 4.4+abs([abs(rand(10,1))*1-0.3; 0.8+abs(rand(10,1))*1.3 ;1+abs(rand(1,1))*1.5; 3+abs(rand(40,1))*3]);
radar = radar*1.02;
[f  xi]= ksdensity(radar,x,'function','cdf');
Ymatrix = [Ymatrix; f];

%% r3.coherent
radar = 4.4+abs([abs(rand(10,1))*1-0.3; 0.8+abs(rand(10,1))*1.3 ;1+abs(rand(10,1))*1.5; 3+abs(rand(90,1))*3]);
radar = radar*1.02;
[f  xi]= ksdensity(radar,x,'function','cdf');
Ymatrix = [Ymatrix; f];

%% r1.Pedo
radar = 2+abs([abs(rand(40,1))*1-0.3; 0.8+abs(rand(50,1))*1.3 ;1+abs(rand(10,1))*1.5; 3+abs(rand(20,1))*3]);
radar = radar*1.02;
[f  xi]= ksdensity(radar,x,'function','cdf');
Ymatrix = [Ymatrix; f];

%% r2.Pedo
radar = 3+abs([abs(rand(10,1))*1-0.3; 0.8+abs(rand(10,1))*1.3 ;1+abs(rand(40,1))*1.5; 3+abs(rand(30,1))*3]);
radar = radar*1.02;
[f  xi]= ksdensity(radar,x,'function','cdf');
Ymatrix = [Ymatrix; f];

%% r3.Pedo
radar = 4+abs([abs(rand(1,1))*1-0.3; 0.8+abs(rand(5,1))*1.3 ;1+abs(rand(10,1))*1.5; 4+abs(rand(50,1))*3]);
radar = radar*1.02;
[f  xi]= ksdensity(radar,x,'function','cdf');
Ymatrix = [Ymatrix; f];



plot1 = plot(x,Ymatrix,'Parent',axes1,'MarkerSize',7,'LineWidth',1.3);
set(plot1(1),'Marker','o','Color',[1 0.400000005960464 0.400000005960464],...
    'DisplayName','R1.CSI');
set(plot1(2),'Marker','square','Color',[1 0 0],'DisplayName','R2.CSI');
set(plot1(3),'Marker','+','Color',[0.600000023841858 0 0],...
    'DisplayName','R3.CSI');
set(plot1(4),'Marker','o','Color',[0.400000005960464 1 0.400000005960464],...
    'DisplayName','R1.Chnt');
set(plot1(5),'Marker','square','Color',[0 0.800000011920929 0],...
    'DisplayName','R2.Chnt');
set(plot1(6),'Marker','+','Color',[0 0.600000023841858 0.200000002980232],...
    'DisplayName','R3.Chnt');
set(plot1(7),'Marker','o','Color',[0.400000005960464 0.400000005960464 1],...
    'DisplayName','R1.Pedo');
set(plot1(8),'Marker','square',...
    'Color',[0 0 0.800000011920929],...
    'DisplayName','R2.Pedo');
set(plot1(9),'Marker','+','Color',[0.200000002980232 0 0.800000011920929],...
    'DisplayName','R3.Pedo');

legend1 = legend(axes1,'show');set(legend1,...
    'Position',[0.657738095238096 0.160576923076923 0.321666666666667 0.626923076923077]);
