function [sr sc] = LocatePupilCenter(original,angleSegs)
img = original;
% img= UniformLightEuqualizeRGBImage(original,100);
imglarge = imresize(img,[ 150/size(img,2)*size(img,1),150]);
imggrey = rgb2gray(imglarge);
hgaussian  = fspecial('gaussian',5,5);
imgfiltered = imfilter(imglarge,hgaussian,'replicate');
imggreyfiltered = double(imfilter(imggrey,hgaussian,'replicate'));
    [FX FY] = gradient(double(imggreyfiltered));
    G1=FX.^2 + FY.^2;
I = imgfiltered;
l1 = I(:,:,1);
l2 = I(:,:,2);
l3 = I(:,:,3);
colors = [l1(:) l2(:) l3(:)];
coords = [];
[coords(:,1) coords(:,2)] = find(l1 >-1);
slots = double([colors coords*0.3]);

clusteringTimes = 3;
currentSlots = slots;
layerTable = zeros(length(slots),1);
currentIndex = 1:length(slots);
for ind = 1:clusteringTimes
    idx = kmeans(currentSlots,2,'emptyaction','drop','replicates',3,'start','uniform');
    if mean(mean(currentSlots(idx==1,1:3))) < mean(mean(currentSlots(idx==2,1:3)))
        ti = 1;
    else
        ti = 2;
    end
    
    currentSlots = currentSlots(idx == ti,:);
    currentIndex = currentIndex(idx == ti);
    layerTable(currentIndex) = layerTable(currentIndex)+1;
end

layerTable = reshape(layerTable,size(l1));
layerTable2 = zeros(size(layerTable));
for ind = 1:clusteringTimes
    clayer = layerTable >= ind;
    clayer = imfill(clayer,'holes');
    clayer = imerode(clayer,strel('disk',3,8));
    cc = bwconncomp(clayer);
    inx = find(cellfun(@(x) length(x),cc.PixelIdxList) == max(cellfun(@(x) length(x),cc.PixelIdxList)));
    leftblob = cc.PixelIdxList{inx};
    clayer  = zeros(size(clayer));
    clayer(leftblob) = 1;
    layerTable2 = layerTable2 + clayer;
end

massCenter = [];
currentSolidity = 0;
bestLayer = 0;
for i = 1:clusteringTimes
    clayer = layerTable2 >= i;
    
    ss = regionprops(clayer,'Solidity');
    if ~isempty(ss) && currentSolidity < max(ss.Solidity) 
        currentSolidity = ss.Solidity;
        [r c] = find(clayer);
        cc = round(mean(c));
        cr = round(mean(r));
        massCenter = [cr cc];
        bestLayer = layerTable2 >= i;
    end
end
%% optimize the bestLayer
cutBestLayer = bestLayer;
if bestLayer(cr,cc) >0
    bestLayerDist = bwdistgeodesic(bestLayer,cc,cr);
    bestLayerDist(isnan(bestLayerDist)) = inf;
    cutBestLayer = bestLayerDist < 50;
    [r c] = find(cutBestLayer);
    cc = round(mean(c));
    cr = round(mean(r));
    massCenter = [cr cc];
    bestLayerDist = bwdistgeodesic(cutBestLayer,cc,cr);
    bestLayerDist(isnan(bestLayerDist)) = inf;
    cutBestLayer = bestLayerDist < 40;
end
sbb = regionprops(cutBestLayer,'BoundingBox');
sbb = sbb.BoundingBox;
%%
rbase = cutBestLayer;
angles = linspace(0,360,angleSegs+1)';
angles(end) =[];
rotatedBase = cell(size(angles));
% rotatedGrey = cell(size(angles));
for i = 1:length(angles)
    rotatedBase{i} = imrotate(rbase,angles(i));
%     rotatedGrey{i} = imrotate(imggreyfiltered,angles(i));
end

%% linear scan solve
% areas = [];
% 
% for i = 1:2:size(rbase,1)
%     for j =1:2:size(rbase,2)
%         areas(i,j) = CircularPupilFinder(angles,rbase, rotatedBase,i,j);
%     end
% end
% 
% areas(areas==0) = nan;
% figure;plot(areas(:,end));
% [~,ind] = nanmin(areas(:));
% [sr sc] = ind2sub(size(areas),ind);


%% optimization problem definition
testf = @(x) CircularPupilFinder(angles, rbase, rotatedBase, x(1),x(2));
options = psoptimset('Cache','on','Display','none','TolMesh',0.125);
th = sbb(2)-2;
if th <1
    th = 1;
end
r = patternsearch(testf,massCenter,[],[],[],[],[th sbb(1)],[cr+2 sbb(1)+sbb(3)],options);
sc = r(2) / (150/size(img,2));
sr = r(1) / (150/size(img,2));
