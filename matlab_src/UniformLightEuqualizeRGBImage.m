function out = UniformLightEuqualizeRGBImage(img,n)

lefteq = EqualizeRGBImage(img(:,1:round(size(img,2)/2)-1,:),n);
righteq = EqualizeRGBImage(img(:,round(size(img,2)/2):end,:),n);
wholeeq = EqualizeRGBImage(img,n);


leftmap = ones(size(lefteq,1),size(lefteq,2));
lm = round(size(lefteq,2)/2)+1:size(lefteq,2);
leftmap(:,lm) = ones(size(lefteq,1),1)*linspace(1,0,length(lm));
leftmap = cat(3,leftmap,leftmap,leftmap);

rightmap = ones(size(righteq,1),size(righteq,2));
lm = 1:round(size(lefteq,2)/2);
rightmap(:,lm) = ones(size(righteq,1),1)*linspace(0,1,length(lm));
rightmap = cat(3,rightmap,rightmap,rightmap);

wholemap = zeros(size(img,1),size(img,2));
lm = round(size(img,2)/4:size(img,2)/2);
wholemap(:,lm) = ones(size(img,1),1)*linspace(0,1,length(lm));
lm = round(size(img,2)/2:size(img,2)/4*3);
wholemap(:,lm) = ones(size(img,1),1)*linspace(1,0,length(lm));
wholemap = cat(3,wholemap,wholemap,wholemap);

out = uint8([double(lefteq).*leftmap double(righteq).*rightmap] + double(wholeeq).*wholemap);