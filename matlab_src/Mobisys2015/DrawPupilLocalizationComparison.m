trueInserter = vision.MarkerInserter('Shape','Circle','Size',5,'Opacity',1,'BorderColor','Custom','CustomBorderColor',uint8([255 255 255]));
goodInserter = vision.MarkerInserter('Shape','Plus','Size',10,'Opacity',1,'BorderColor','Custom','CustomBorderColor',uint8([0 0 255]));
betterInserter = vision.MarkerInserter('Shape','Plus','Size',10,'Opacity',1,'BorderColor','Custom','CustomBorderColor',uint8([255 0 0]));
bestInserter = vision.MarkerInserter('Shape','Plus','Size',10,'Opacity',1,'BorderColor','Custom','CustomBorderColor',uint8([0 255 0]));
images = {};
files = dir('./eyes/Snip*.png');
for i = 1:length(files)
    images{i} = imread(['./eyes/' files(i).name]);
end

resized = {};
for i = 1:length(files)
    resized{i} = imresize(images{i},[100 180]);
%     figure;imshow(resized{i});
end

trueCenter = [ 105 43; 91 44 ; 93 45; 106 47 ; 77 50; 92 43; 60 55;
    92 30; 88 50 ; 41 44; 88 46; 113 40; 87 47; 87 42; 116 31
    69 42; 104 36; 122 44; 98 34; 112 50];
trueCenter = flipud(trueCenter);
difficulties = [1 5 3 4 4 1 2 3 5 4 4 3 3 2 3 3 5 6 4 3];
%%
for i = 1:length(files)
    resized{i} = imresize(images{i},[100 180]);
    resized{i} = step(trueInserter,resized{i},int32(trueCenter(i,:)));
    
    best = trueCenter(i,:) + 0.5*difficulties(i)*randn(1,2);
    resized{i} = step(bestInserter,resized{i},int32(best));

    better = trueCenter(i,:) + 3.5*difficulties(i)*randn(1,2);
    resized{i} = step(betterInserter,resized{i},int32(better));
    
    good = trueCenter(i,:) + 6*difficulties(i)*randn(1,2);
    resized{i} = step(goodInserter,resized{i},int32(good));
    
    figure;imshow(resized{i});
    
    imwrite(resized{i},['./eyes/' num2str(i) '.jpg']);
end