function axis = optimalSubplot(figure,cols,rows,col,row)

colpad = 0.03;
rowpad = 0.03;
width = 1/cols-0.04;
height = 1/rows-0.06;
axis = subplot('Position',[1/cols*(col-1)+colpad 1/rows*(rows-row)+rowpad width height],'Parent',figure);