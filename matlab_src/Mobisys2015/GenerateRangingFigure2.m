clc;clear;close all;

stepDistance = 0.5;
minDistance = 1;
maxDistance = 30;

stepTime = 10;
minTime = 10;
maxTime = 300;

cToObAeRate = 0.001;
cToFaceAeRate = 0.01;
faceToObAeRate = 0.6;

cp = [0 0];
fp = [-0.5 -0.5];

[meshDistance meshTime] = meshgrid(minDistance:stepDistance:maxDistance,minTime:stepTime:maxTime);

rawpool = cell(size(meshDistance));

for i = 1:size(meshDistance,1)
    for j = 1:size(meshDistance,2)
        slot = zeros(meshTime(i,j),1);
        for k = 1:meshTime(i,j)
            cToObAe = randn*cToObAeRate;
            while abs(cToObAe) > cToObAeRate
                cToObAe = randn*cToObAeRate;
            end
            cToFaceAe = randn*cToFaceAeRate;
            while abs(cToFaceAe) > cToFaceAeRate
                cToFaceAe = randn*cToFaceAeRate;
            end
            faceToObAe = randn*faceToObAeRate;
            while abs(faceToObAe) > faceToObAeRate
                faceToObAe = randn*faceToObAeRate;
            end
            [~,slot(k)] = RangeErrorConduction([0 meshDistance(i,j)],cp,fp,cToObAe,cToFaceAe,faceToObAe);
        end
        rawpool{i,j} = slot;
    end
end

%%
med = cellfun(@(x) median(x),rawpool);
means = cellfun(@(x) mean(x),rawpool);
stds = cellfun(@(x) std(x),rawpool);
mads = cellfun(@(x) mad(x),rawpool);
percent10 = cellfun(@(x) prctile(x,11),rawpool);
%%

finalMed = abs(med);
finalMed = finalMed;

figureSize = [500 500 430 260 ];
axesPosition = [0.093333333333333 0.153333333333333 0.781666666666667 0.801666666666667];
colorbarPosition = [0.900172839506176 0.153333333333333 0.0268971193415638 0.801666666666667];


figure1 = figure('Colormap',...
    [0.368627458810806 0.309803932905197 0.635294139385223;0.316435217857361 0.345023304224014 0.66557902097702;0.274142324924469 0.380341947078705 0.690980136394501;0.265949815511703 0.389196336269379 0.696074187755585;0.257757306098938 0.39805069565773 0.701168179512024;0.249564826488495 0.406905055046082 0.706262230873108;0.241372317075729 0.415759444236755 0.711356282234192;0.236647620797157 0.422862619161606 0.714398324489594;0.217748835682869 0.451275378465652 0.72656637430191;0.205371037125587 0.480953633785248 0.734818756580353;0.202895477414131 0.48688930273056 0.736469209194183;0.199665650725365 0.504745125770569 0.738696455955505;0.196435824036598 0.522600889205933 0.740923643112183;0.214933633804321 0.561481237411499 0.728923559188843;0.233431443572044 0.600361585617065 0.716923415660858;0.251929253339767 0.639241933822632 0.704923331737518;0.334348231554031 0.714649140834808 0.663251340389252;0.376671820878983 0.745573163032532 0.65004426240921;0.45473513007164 0.778357148170471 0.643155515193939;0.53279846906662 0.811141192913055 0.636266767978668;0.610861778259277 0.843925178050995 0.629377961158752;0.688925087451935 0.876709163188934 0.622489213943481;0.766988396644592 0.909493207931519 0.61560046672821;0.84505170583725 0.942277193069458 0.608711719512939;0.877370715141296 0.954767942428589 0.600337445735931;0.899151563644409 0.960602998733521 0.596210658550262;0.911125719547272 0.960570633411407 0.6011922955513;0.920339465141296 0.95973926782608 0.618974447250366;0.926913619041443 0.957870721817017 0.644331336021423;0.933487713336945 0.956002175807953 0.66968822479248;0.940061867237091 0.954133629798889 0.695045113563538;0.946437478065491 0.951453864574432 0.70950174331665;0.953988790512085 0.948066353797913 0.70954954624176;0.963093936443329 0.941698968410492 0.694903016090393;0.972800314426422 0.932388484477997 0.669458448886871;0.980100274085999 0.918754100799561 0.635803878307343;0.98740029335022 0.905119717121124 0.602149367332458;0.994700253009796 0.891485333442688 0.56849479675293;0.996073722839355 0.874946415424347 0.539816379547119;0.995913088321686 0.853220283985138 0.512809693813324;0.993664801120758 0.816369473934174 0.48790043592453;0.99141651391983 0.779518604278564 0.462991178035736;0.989168286323547 0.7426677942276 0.438081920146942;0.98691999912262 0.705816984176636 0.413172662258148;0.984671711921692 0.668966114521027 0.388263374567032;0.982423424720764 0.632115304470062 0.363354116678238;0.980175197124481 0.595264494419098 0.338444858789444;0.977926909923553 0.558413624763489 0.31353560090065;0.975678622722626 0.521562814712524 0.288626343011856;0.968399882316589 0.479865729808807 0.27232038974762;0.9602912068367 0.441479504108429 0.26358950138092;0.950775921344757 0.407784014940262 0.263944298028946;0.931123495101929 0.37930503487587 0.272793471813202;0.911471128463745 0.350826025009155 0.281642615795135;0.891818702220917 0.322347044944763 0.290491789579391;0.872166335582733 0.293868035078049 0.299340933561325;0.852513909339905 0.265389055013657 0.308190107345581;0.819241642951965 0.228036552667618 0.301137745380402;0.785969316959381 0.19068406522274 0.294085383415222;0.752697050571442 0.153331562876701 0.287033021450043;0.719424724578857 0.115979060530663 0.279980629682541;0.686152458190918 0.0786265656352043 0.272928267717361;0.652880132198334 0.0412740670144558 0.265875905752182;0.619607865810394 0.00392156885936856 0.258823543787003]);
set(figure1,'Position',figureSize);
% set(figure1,'Colormap',linspecer);
set(figure1,'DefaultAxesFontSize',14);

% Create axes
axes1 = axes('Parent',figure1,'YTickLabel',{'1','2','3','4','5','6'},...
    'YDir','reverse',...
    'XTickLabel',{'0','5','10','15','20','25','30'},...
    'XTick',[1 11 21 31 41 51 59],...
    'Position',axesPosition,...
    'CLim',[0 120]);
xlim(axes1,[1 59]);
ylim(axes1,[1 30]);
grid(axes1,'on');
hold(axes1,'all');

imagesc(finalMed,'Parent',axes1);

xlabel('Target Object Distance (m)');

% Create ylabel
ylabel('Gazing time (s)');

colorbar('peer',axes1,'Position',colorbarPosition);

save2pdf('RangingErrorWithDistance.pdf');
%%

%%
ErrorWithDistanece =meshDistance + finalMed;
pts = 0:0.1:60;
fs = [];
xs = [];
for i = 1:6:size(ErrorWithDistanece,2)
    [f x] = ksdensity(ErrorWithDistanece(:,i),pts,'function','cdf');
     fs = [fs  f'];
     xs = [xs  x'];

end

fs(:,[1:2]) = [];

%%



figureSize = [500 500 450 280 ];
axesPosition = [0.103333333333333 0.143333333333333 0.861666666666667 0.821666666666667];
colorbarPosition = [0.890172839506176 0.133333333333333 0.0468971193415638 0.811666666666667];


figure1 = figure;
set(figure1,'Position',figureSize);
set(figure1,'Colormap',linspecer);
set(figure1,'DefaultAxesFontSize',14);

axes1 = axes('Parent',figure1,...
    'XTickLabel',{'2','7','12','17','22','27','32'},...
    'XTick',[7.4 12.4 17.4 22.4 27.4 32.4 37.4],...
    'CLim',[1 100],...
    'XGrid','on',...
    'Position',axesPosition);
xlim([6.5 40]);
ylim([0 1]);
hold(axes1,'all');
box(axes1,'on');

colormap(axes1,[cool(32); linspecer(32); autumn(32); summer(32); spring(32); winter(32); cool(32); linspecer(32); autumn(32); summer(32);]);
for i = 1:size(fs,2)
    zmin = min(fs(:,i));
    zmax = max(fs(:,i));
    % CDX ranges from 1 to 32.
    cdx = min(32*i,round(31*(fs(:,i)-zmin)/(zmax-zmin))+32*(i-1)+1);
    
    h1 = scatter(xs(:,i),fs(:,i),70,1:length(xs(:,i)),'o','filled');
    set(h1,'CData',cdx);
    
end
caxis([1 64*5]);

rgb = [151 32 62];rgb = normr(rgb);

errorRow = [78 123 178 240 330 370 390 400];
errorCol = 1:8;
errorIndex = sub2ind(size(fs),errorRow,errorCol);
errorX = xs(errorRow,1);
errorV = fs(errorIndex);
plot(errorX,errorV,'Color',rgb,'MarkerSize',30,'Marker','.');


t1index = 80; t2index = 130, t3index =190, t4index=260, t5index=310, t6index = 335, t7index = 360 , t8index = 380;
text(xs(t1index,1),fs(t1index,1),'2m','FontSize',14,'FontWeight','bold','HorizontalAlignment','center','BackgroundColor',[1,1,1],'Margin',0.5);
text(xs(t2index,2),fs(t2index,2),'7m','FontSize',14,'FontWeight','bold','HorizontalAlignment','center','BackgroundColor',[1,1,1],'Margin',0.5);
text(xs(t3index,3),fs(t3index,3),'12m','FontSize',14,'FontWeight','bold','HorizontalAlignment','center','BackgroundColor',[1,1,1],'Margin',0.5);
text(xs(t4index,4),fs(t4index,4),'17m','FontSize',14,'FontWeight','bold','HorizontalAlignment','center','BackgroundColor',[1,1,1],'Margin',0.5);
text(xs(t5index,5),fs(t5index,5),'22m','FontSize',14,'FontWeight','bold','HorizontalAlignment','center','BackgroundColor',[1,1,1],'Margin',0.5);
text(xs(t6index,6),fs(t6index,6),'27m','FontSize',14,'FontWeight','bold','HorizontalAlignment','center','BackgroundColor',[1,1,1],'Margin',0.5);
text(xs(t7index,7),fs(t7index,7),'32m','FontSize',14,'FontWeight','bold','HorizontalAlignment','center','BackgroundColor',[1,1,1],'Margin',0.1);
text(xs(t8index,8),fs(t8index,8),'37m','FontSize',14,'FontWeight','bold','HorizontalAlignment','center','BackgroundColor',[1,1,1],'Margin',0.1);


% Create xlabel
xlabel('Distance (m)');

% Create ylabel
ylabel('CDF (%)');

save2pdf('FinalStrike.pdf');