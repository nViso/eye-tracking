function pack = ParseFusedData(pathHeader,gap1Model,gap2Model,gap3Model,density,threshold)


traceName = pathHeader;
visionTraceName = [traceName 'vision.test'];
sensorTraceName = [traceName 'sensor.txt'];

if nargin == 1
    pack = ParseFusedDataWithFileNames(sensorTraceName,visionTraceName);
else 
    pack = ParseFusedDataWithFileNames(sensorTraceName,visionTraceName,gap1Model,gap2Model,gap3Model,density,threshold);
end