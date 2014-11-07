function pack = ParseFusedDataWithFileNames(sensorTraceName,visionTraceName,gap1Model,gap2Model,gap3Model,density,threshold)

withModel = false;

if nargin == 4
    withModel = true;
end


sensorData = sensorTextReadFromDisk(sensorTraceName);
sensorData = DeviceMotionAxesShift(sensorData);

if withModel == true
    pack = ParseFusedDataWithSensorData(sensorData,visionTraceName,gap1Model,gap2Model,gap3Model,density,threshold);
else
    pack = ParseFusedDataWithSensorData(sensorData,visionTraceName);
end