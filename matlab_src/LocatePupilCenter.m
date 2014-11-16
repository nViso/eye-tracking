function [sr sc] = LocatePupilCenter(img)

imglarge = imresize(img,[ 150/size(img,2)*size(img,1),150]);
imggrey = rgb2gray(imglarge);
hgaussian  = fspecial('gaussian',5,5);
imgfiltered = imfilter(imglarge,hgaussian,'replicate');
imggreyfiltered = double(imfilter(imggrey,hgaussian,'replicate'));
%     [FX FY] = gradient(double(imggreyfiltered));
%     G1=FX.^2 + FY.^2;
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
    
    s = regionprops(clayer,'Solidity');
    if ~isempty(s) && currentSolidity < max(s.Solidity) 
        currentSolidity = s.Solidity;
        [x y] = find(clayer);
        cy = round(mean(y));
        cx = round(mean(x));
        massCenter = [cx cy];
        bestLayer = layerTable2 >= i;
    end
end


%%
rbase = bestLayer;
angles = [0:40:359]';
rotatedBase = cell(size(angles));
rotatedGrey = cell(size(angles));
for i = 1:length(angles)
    rotatedBase{i} = imrotate(rbase,angles(i));
    rotatedGrey{i} = imrotate(imggreyfiltered,angles(i));
end

dcms = angle2dcm(deg2rad(angles),zeros(size(angles)),zeros(size(angles)));

areas = [];
solidities = [];
highestScore = [];
slope = [];
for i = 1:2:size(imgfiltered,1)
    for j =1:2:size(imgfiltered,2)
        cornerPoints = [[-j; -i ; 0] [-j; size(rbase,1)-i ; 0] [size(rbase,2)-j;size(rbase,1)-i;0] [size(rbase,2)-j; -i ; 0] ];
        
        ranges = {};
        for a = 1:length(angles)
            rcorners = dcms(:,:,a)*cornerPoints+[j j j j ; i i i i; 0 0 0 0];
            rcorners(3,:) = [];
            rcorners = sort(rcorners,2,'ascend');
            ranges{a} = rcorners(:,[1 end]);
        end
        xrefs = cell2mat(cellfun(@(x) x(1,:),ranges','UniformOutput',false));
        xlims = [min(xrefs(:,1)) max(xrefs(:,2))];
        yrefs = cell2mat(cellfun(@(x) x(2,:),ranges','UniformOutput',false));
        ylims = [min(yrefs(:,1)) max(yrefs(:,2))];
        template = zeros(ceil(ylims(2)-ylims(1)+10),ceil(xlims(2)-xlims(1)+10));
        greytemp = zeros(ceil(ylims(2)-ylims(1)+10),ceil(xlims(2)-xlims(1))+10);
        for a = 1:length(angles)
            [m, n] = worldToSubscript(xlims,ylims,ranges{a}(1,1),ranges{a}(2,1));
        
            template(m:m+size(rotatedBase{a},1)-1,n:n+size(rotatedBase{a},2)-1) = template(m:m+size(rotatedBase{a},1)-1,n:n+size(rotatedBase{a},2)-1) + rotatedBase{a};
            greytemp(m:m+size(rotatedBase{a},1)-1,n:n+size(rotatedBase{a},2)-1) = greytemp(m:m+size(rotatedBase{a},1)-1,n:n+size(rotatedBase{a},2)-1) + 1/length(angles)*rotatedBase{a}.*rotatedGrey{a};

            
        end
        
        highestScore(i,j) = nanmax(nanmax(greytemp));
        
        st = template;
        st(st<3) = 0;
        s = regionprops(st,'BoundingBox');
        if ~isempty(s)
            s = s(end);
            s = s.BoundingBox(3:4);
        else
            s = [1 1];
        end
        slope(i,j) = bwarea(st) / (s(1)*s(2));
        
        template(template>1) = 1;
        areas(i,j) = bwarea(template);
    end
end

areas(areas==0) = nan;
[~,ind] = nanmin(areas(:));
[sr sc] = ind2sub(size(areas),ind);

sc = sc / (150/size(img,2));
sr = sr / (150/size(img,2));
