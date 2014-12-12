function [lm, lv] = ScanViewZLayer 

[lm, lv] = ScanViewError(0.2);

end


function [m, v] = ScanViewError(fToObAE)
fToObAEDist = makedist('Normal','mu',0,'sigma',fToObAE);
cTofAEDist = makedist('Normal','mu',0,'sigma',0.001);
cToObAEDist = makedist('Normal','mu',0,'sigma',0.001);
obp = [0 5]; cp = [0 0] ; 
trials = 30;
grid = 0.02;
xrange = [-0.4 0.4];
yrange = [-1.0 -0.2];
zrange = [-0.4 0.4];
[gridsx gridsy gridsz] = meshgrid(xrange(1):grid:xrange(2),yrange(1):grid:yrange(2),zrange(1):grid:zrange(2));
results = zeros([size(gridsx) trials]);

for z = 1:size(gridsz,3)
    display(z);
    for x = 1:size(gridsx,2)
        parfor y = 1:size(gridsy,1)
            for i = 1:trials
                p = [gridsx(1,x,1) gridsy(y,1,1) gridsz(1,1,z)];
                equalizedx = norm([p(1) p(3)]);
                results(z,y,x,i) = RangeErrorConduction(obp,cp,[equalizedx p(2)],...
                    0,0,...
                    randn*fToObAE*(1+p(3)*0.6)*(1-p(2)*0.5));
            end
        end
    end
end
%%
results(isinf(results)) = nan;
v = nanstd(results,1,4);
m = nanmedian(results,4);

end