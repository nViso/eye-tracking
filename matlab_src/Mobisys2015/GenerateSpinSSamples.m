addpath('../');
addpath('../fusing');
shapeInserter = vision.ShapeInserter('Shape','Circles','Fill',true,'FillColor','Custom','CustomFillColor',uint8([255]));
base = uint8(zeros(70,70));
base = step(shapeInserter,base,int32([35 35 28]));
base = base > 1;
base(1:40,:) = 0;
base = uint8(zeros(70,70));
base(20,70) = 1;
base(70,20) = 1;
base(50,40) = 1;
base(30,30) = 1;
base = bwconvhull(base);
angleSegs = 7;
scores = nan(70,70,30);
%%
for angleSegs = 7
    rbase = base;
    angles = linspace(0,360,angleSegs+1)';
    angles(end) =[];
    rotatedBase = cell(size(angles));
    rotatedGrey = cell(size(angles));
    for i = 1:length(angles)
        rotatedBase{i} = imrotate(rbase,angles(i));
        %     rotatedGrey{i} = imrotate(imggreyfiltered,angles(i));
    end
    
    score = zeros(size(rbase));
    
    for i = 1:size(rbase,1)
        parfor j =1:size(rbase,2)
            score(i,j) = CircularPupilFinder(angles,rbase, rotatedBase,i,j);
        end
    end
    
    score(score==0) = nan;
    
    %% interpolate the areas.
    [kr kc] = find(~isnan(score));
    [ur uc] = find(isnan(score));
    ki = sub2ind(size(score),kr,kc);
    ui = sub2ind(size(score),ur,uc);
    kv = score(ki);
    uv = griddata(kr,kc,kv,ur,uc);
    score(ui) = uv;
    
    [kr kc] = find(~isnan(score));
    [ur uc] = find(isnan(score));
    ki = sub2ind(size(score),kr,kc);
    ui = sub2ind(size(score),ur,uc);
    kv = score(ki);
    uv = griddata(kr,kc,kv,ur,uc,'nearest');
    score(ui) = uv;
    scores(:,:,angleSegs) = score;
    scores(:,:,angleSegs);
    [~,i] = min(ans(:));
    [r c] = ind2sub(size(scores(:,:,angleSegs)),i)
end

