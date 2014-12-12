function sequenceGenerator
heights = [];
for i = 0:10
    for j = 0:10
        heights = [ heights; i+j j];
    end
end
        WriteSequence(heights);
end

function WriteSequence(NoseAndPhiltrumHeight,fileName)

if nargin <3
    fileName = 'ScanSequence.txt';
end

dlmwrite(fileName,NoseAndPhiltrumHeight,'delimiter',' ');

end