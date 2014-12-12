function out = EqualizeRGBImage(img,n)
ycc = rgb2ycbcr(img);

ycc(:,:,1) = histeq(ycc(:,:,1),n);
out = ycbcr2rgb(ycc);