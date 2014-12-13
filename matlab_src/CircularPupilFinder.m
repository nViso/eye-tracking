function out = CircularPupilFinder(angles, rbase, rotatedBase, i,j)

dcms = angle2dcm(deg2rad(angles),zeros(size(angles)),zeros(size(angles)));


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
% greytemp = zeros(ceil(ylims(2)-ylims(1)+10),ceil(xlims(2)-xlims(1))+10);
for a = 1:length(angles)
    [m, n] = worldToSubscript(xlims,ylims,ranges{a}(1,1),ranges{a}(2,1));
    template(m:m+size(rotatedBase{a},1)-1,n:n+size(rotatedBase{a},2)-1) = template(m:m+size(rotatedBase{a},1)-1,n:n+size(rotatedBase{a},2)-1) + rotatedBase{a};
    %     greytemp(m:m+size(rotatedBase{a},1)-1,n:n+size(rotatedBase{a},2)-1) = greytemp(m:m+size(rotatedBase{a},1)-1,n:n+size(rotatedBase{a},2)-1) + 1/length(angles)*rotatedBase{a}.*rotatedGrey{a};
    
end
% figure;imagesc(template);


l = template > 0;
f = imfill(l,'holes');
a = bwarea(l);
bcv = bwconvhull(l);

fa = bwarea(f);
bcva = bwarea(bcv);
out  =- ( a /bcva);