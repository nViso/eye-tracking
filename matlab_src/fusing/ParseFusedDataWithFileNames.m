function pack = ParseFusedDataWithFileNames(sensorTraceName,visionTraceName,gap1Model,gap2Model,gap3Model,density,threshold)

sensorData = sensorTextReadFromDisk(sensorTraceName);
sensorData = DeviceMotionAxesShift(sensorData);

ParseFusedDataWithSensorData(sensorData,visionTraceName,gap1Model,gap2Model,gap3Model,density,threshold)