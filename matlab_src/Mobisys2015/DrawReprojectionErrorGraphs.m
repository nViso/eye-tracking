close all;
if exist('xcombine') == 0
    load('HeightScanReprojectionErrorGraphs.mat')
end
figureSize = [500 500 300 250 ];
axesPosition = [0.116666666666667 0.133333333333333 0.738333333333335 0.811666666666667];
colorbarPosition = [0.890172839506176 0.133333333333333 0.0468971193415638 0.811666666666667];
fontSize = 13;
figure1 = figure;
set(figure1,'Position',figureSize);
set(figure1,'Colormap',parula);
% Create axes
axes1 = axes('YTickLabel',{'0','5','10','15','20','25'},...
    'YTick',[0.5 16 31.5 47 62.5 78],...
    'XTickLabel',{'0','5','10','15','20','25'},...
    'XTick',[0.5 16 31.5 47 62.5 78],...
    'Position',axesPosition,...
    'Layer','top',...
    'FontSize',fontSize);
xlim(axes1,[0.5 78]);
ylim(axes1,[0.5 78]);
box(axes1,'on');
hold(axes1,'all');

% Create image
imagesc(xreprojectionRMS);

% Create xlabel
xlabel('Mid UpperLip Point Height (mm)','FontSize',fontSize);

% Create ylabel
ylabel('Nose Points Height (mm)','FontSize',fontSize);

% Create colorbar
colorbar('peer',axes1,colorbarPosition,'FontSize',fontSize);

save2eps('ReprojectionErrorMean.eps',figure1);
% print(figure1,'ReprojectionErrorMean.eps','-depsc2');
% print -painters -dpdf -r600 test.pdf
%%

figure1 = figure;
set(figure1,'Position',figureSize);
set(figure1,'Colormap',linspecer);
% Create axes
axes1 = axes('YTickLabel',{'0','5','10','15','20','25'},...
    'YTick',[0.5 16 31.5 47 62.5 78],...
    'XTickLabel',{'0','5','10','15','20','25'},...
    'XTick',[0.5 16 31.5 47 62.5 78],...
    'Position',axesPosition,...
    'Layer','top',...
    'FontSize',fontSize);
xlim(axes1,[0.5 78]);
ylim(axes1,[0.5 78]);
box(axes1,'on');
hold(axes1,'all');

% Create image
imagesc(xreprojectionRMSstd);

% Create xlabel
xlabel('Mid UpperLip Point Height (mm)','FontSize',fontSize);

% Create ylabel
ylabel('Nose Points Height (mm)','FontSize',fontSize);

% Create colorbar
colorbar('peer',axes1,colorbarPosition,'FontSize',fontSize);

save2pdf('ReprojectionErrorStd.eps',figure1);

%%
figure1 = figure;
set(figure1,'Position',figureSize);
set(figure1,'Colormap',...
    [0.368627458810806 0.309803932905197 0.635294139385223;0.32620906829834 0.345122426748276 0.660648167133331;0.283790707588196 0.380440950393677 0.686002254486084;0.262581527233124 0.398100197315216 0.698679268360138;0.241372317075729 0.415759444236755 0.711356282234192;0.217748835682869 0.451275378465652 0.72656637430191;0.218997076153755 0.525055050849915 0.725641131401062;0.220245316624641 0.598834753036499 0.724715948104858;0.23080663383007 0.612303793430328 0.71811842918396;0.241367936134338 0.625772893428802 0.711520850658417;0.251929253339767 0.639241933822632 0.704923331737518;0.279402256011963 0.664377689361572 0.691032648086548;0.306875228881836 0.689513385295868 0.677142024040222;0.334348231554031 0.714649140834808 0.663251340389252;0.344929128885269 0.7223801612854 0.659949541091919;0.355510026216507 0.730111122131348 0.656647801399231;0.366090923547745 0.73784214258194 0.653346061706543;0.376671820878983 0.745573163032532 0.65004426240921;0.45473513007164 0.778357148170471 0.643155515193939;0.53279846906662 0.811141192913055 0.636266767978668;0.610861778259277 0.843925178050995 0.629377961158752;0.688925087451935 0.876709163188934 0.622489213943481;0.766988396644592 0.909493207931519 0.61560046672821;0.84505170583725 0.942277193069458 0.608711719512939;0.877370715141296 0.954767942428589 0.600337445735931;0.899151563644409 0.960602998733521 0.596210658550262;0.911125719547272 0.960570633411407 0.6011922955513;0.920339465141296 0.95973926782608 0.618974447250366;0.926913619041443 0.957870721817017 0.644331336021423;0.933487713336945 0.956002175807953 0.66968822479248;0.940061867237091 0.954133629798889 0.695045113563538;0.946437478065491 0.951453864574432 0.70950174331665;0.953988790512085 0.948066353797913 0.70954954624176;0.963093936443329 0.941698968410492 0.694903016090393;0.972800314426422 0.932388484477997 0.669458448886871;0.980100274085999 0.918754100799561 0.635803878307343;0.98740029335022 0.905119717121124 0.602149367332458;0.994700253009796 0.891485333442688 0.56849479675293;0.996073722839355 0.874946415424347 0.539816379547119;0.995913088321686 0.853220283985138 0.512809693813324;0.993022441864014 0.805840671062469 0.480783492326736;0.990131795406342 0.758460998535156 0.448757320642471;0.987241148948669 0.711081385612488 0.416731119155884;0.984350562095642 0.663701713085175 0.384704917669296;0.98145991563797 0.616322100162506 0.352678716182709;0.978569269180298 0.568942427635193 0.320652544498444;0.975678622722626 0.521562814712524 0.288626343011856;0.973252356052399 0.507663786411285 0.283191025257111;0.970826148986816 0.493764758110046 0.277755707502365;0.968399882316589 0.479865729808807 0.27232038974762;0.966372728347778 0.470269173383713 0.270137667655945;0.964345574378967 0.460672616958618 0.26795494556427;0.962318360805511 0.451076060533524 0.265772223472595;0.9602912068367 0.441479504108429 0.26358950138092;0.957912385463715 0.433055639266968 0.263678193092346;0.955533564090729 0.424631774425507 0.263766884803772;0.953154742717743 0.416207879781723 0.26385560631752;0.950775921344757 0.407784014940262 0.263944298028946;0.931123495101929 0.37930503487587 0.272793471813202;0.911471128463745 0.350826025009155 0.281642615795135;0.891818702220917 0.322347044944763 0.290491789579391;0.872166335582733 0.293868035078049 0.299340933561325;0.852513909339905 0.265389055013657 0.308190107345581;0.619607865810394 0.00392156885936856 0.258823543787003]);

% Create axes
axes1 = axes('YTickLabel',{'0','5','10','15','20','25'},...
    'YTick',[0.5 16 31.5 47 62.5 78],...
    'XTickLabel',{'0','5','10','15','20','25'},...
    'XTick',[0.5 16 31.5 47 62.5 78],...
    'Position',axesPosition,...
    'Layer','top',...
    'CLim',[0 50.262],...
    'FontSize',fontSize);
xlim(axes1,[0.5 78]);
ylim(axes1,[0.5 78]);
box(axes1,'on');
hold(axes1,'all');

% Create image
imagesc(xcombine/2);
% Create xlabel
xlabel('Mid UpperLip Point Height (mm)','FontSize',fontSize);

% Create ylabel
ylabel('Nose Points Height (mm)','FontSize',fontSize);

% Create colorbar
colorbar('peer',axes1,colorbarPosition,'FontSize',fontSize);

save2pdf('ReprojectionErrorCombine.eps',figure1);

%%
figure1 = figure;
set(figure1,'Position',figureSize);
set(figure1,'Colormap',...
    [0.368627458810806 0.309803932905197 0.635294139385223;0.32620906829834 0.345122426748276 0.660648167133331;0.283790707588196 0.380440950393677 0.686002254486084;0.262581527233124 0.398100197315216 0.698679268360138;0.241372317075729 0.415759444236755 0.711356282234192;0.217748835682869 0.451275378465652 0.72656637430191;0.218997076153755 0.525055050849915 0.725641131401062;0.220245316624641 0.598834753036499 0.724715948104858;0.23080663383007 0.612303793430328 0.71811842918396;0.241367936134338 0.625772893428802 0.711520850658417;0.251929253339767 0.639241933822632 0.704923331737518;0.279402256011963 0.664377689361572 0.691032648086548;0.306875228881836 0.689513385295868 0.677142024040222;0.334348231554031 0.714649140834808 0.663251340389252;0.344929128885269 0.7223801612854 0.659949541091919;0.355510026216507 0.730111122131348 0.656647801399231;0.366090923547745 0.73784214258194 0.653346061706543;0.376671820878983 0.745573163032532 0.65004426240921;0.45473513007164 0.778357148170471 0.643155515193939;0.53279846906662 0.811141192913055 0.636266767978668;0.610861778259277 0.843925178050995 0.629377961158752;0.688925087451935 0.876709163188934 0.622489213943481;0.766988396644592 0.909493207931519 0.61560046672821;0.84505170583725 0.942277193069458 0.608711719512939;0.877370715141296 0.954767942428589 0.600337445735931;0.899151563644409 0.960602998733521 0.596210658550262;0.911125719547272 0.960570633411407 0.6011922955513;0.920339465141296 0.95973926782608 0.618974447250366;0.926913619041443 0.957870721817017 0.644331336021423;0.933487713336945 0.956002175807953 0.66968822479248;0.940061867237091 0.954133629798889 0.695045113563538;0.946437478065491 0.951453864574432 0.70950174331665;0.953988790512085 0.948066353797913 0.70954954624176;0.963093936443329 0.941698968410492 0.694903016090393;0.972800314426422 0.932388484477997 0.669458448886871;0.980100274085999 0.918754100799561 0.635803878307343;0.98740029335022 0.905119717121124 0.602149367332458;0.994700253009796 0.891485333442688 0.56849479675293;0.996073722839355 0.874946415424347 0.539816379547119;0.995913088321686 0.853220283985138 0.512809693813324;0.993022441864014 0.805840671062469 0.480783492326736;0.990131795406342 0.758460998535156 0.448757320642471;0.987241148948669 0.711081385612488 0.416731119155884;0.984350562095642 0.663701713085175 0.384704917669296;0.98145991563797 0.616322100162506 0.352678716182709;0.978569269180298 0.568942427635193 0.320652544498444;0.975678622722626 0.521562814712524 0.288626343011856;0.973252356052399 0.507663786411285 0.283191025257111;0.970826148986816 0.493764758110046 0.277755707502365;0.968399882316589 0.479865729808807 0.27232038974762;0.966372728347778 0.470269173383713 0.270137667655945;0.964345574378967 0.460672616958618 0.26795494556427;0.962318360805511 0.451076060533524 0.265772223472595;0.9602912068367 0.441479504108429 0.26358950138092;0.957912385463715 0.433055639266968 0.263678193092346;0.955533564090729 0.424631774425507 0.263766884803772;0.953154742717743 0.416207879781723 0.26385560631752;0.950775921344757 0.407784014940262 0.263944298028946;0.931123495101929 0.37930503487587 0.272793471813202;0.911471128463745 0.350826025009155 0.281642615795135;0.891818702220917 0.322347044944763 0.290491789579391;0.872166335582733 0.293868035078049 0.299340933561325;0.852513909339905 0.265389055013657 0.308190107345581;0.619607865810394 0.00392156885936856 0.258823543787003]);

% Create axes
axes1 = axes('YTickLabel',{'0','5','10','15','20','25'},...
    'YTick',[0.5 16 31.5 47 62.5 78],...
    'XTickLabel',{'0','5','10','15','20','25'},...
    'XTick',[0.5 16 31.5 47 62.5 78],...
    'Position',axesPosition,...
    'Layer','top',...
    'CLim',[0 50.262],...
    'FontSize',fontSize);
xlim(axes1,[0.5 78]);
ylim(axes1,[0.5 78]);
box(axes1,'on');
hold(axes1,'all');

% Create image
imagesc(xcombineG/2);
text(c,r,'\leftarrow optimal combination','FontSize',fontSize);
% Create xlabel
xlabel('Mid UpperLip Point Height (mm)','FontSize',fontSize);

% Create ylabel
ylabel('Nose Points Height (mm)','FontSize',fontSize);

% Create colorbar
colorbar('peer',axes1,colorbarPosition,'FontSize',fontSize);

save2pdf('ReprojectionErrorCombineG.eps',figure1);