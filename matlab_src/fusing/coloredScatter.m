function coloredScatter(data,width)

if nargin < 2
    width = 20;
end

if size(data,2) == 2
    scatter(data(:,1),data(:,2),width,1:length(data),'filled');
end

if size(data,2) == 3
    scatter3(data(:,1),data(:,2),data(:,3),width,1:length(data),'filled');
end
xlabel('x');
ylabel('y');
zlabel('z');