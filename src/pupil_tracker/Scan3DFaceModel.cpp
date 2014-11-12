//
//  Scan3DFaceModel.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14/11/3.
//
//
#include "ASMGazeTracker.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>

vector<vector<Point2f> > extractFeaturePoints(vector<vector<float> > datas) {
    vector<vector<Point2f> > points;
    for (int i = 0 ; i < datas.size() ; i ++) {
        vector<Point2f> sevenPoints;
        for (int j = 0; j < 18; j +=2) {
            sevenPoints.push_back(Point2f(datas[i][j],datas[i][j+1]));
        }
        points.push_back(sevenPoints);
    }
    return points;
}

int main(int argc, const char * argv[])
{
    vector<vector<float> > rowVectors;
    if (argc != 5 && argc<9) {
        cout<<argv[0]<<" userProfileDir camera_profile videoMetadataFile minNoseHeight maxNoseHeight minPhiltrumHeight maxPhiltrumHeight step"<<endl;
        cout<<argv[0]<<" userProfileDir camera_profile videoMetadataFile sequenceFile"<<endl;
        return 0;
    }
    
    if (argc == 5) {
        fs::path sequenceFile(argv[4]);
        if (fs::exists(sequenceFile)) {
            rowVectors = parseTextTableFile(sequenceFile.string(), " ");
        }
    }
    if (argc == 9) {
        float minNose = boost::lexical_cast<float>(argv[4]);
        float maxNose = boost::lexical_cast<float>(argv[5]);
        float minPhil = boost::lexical_cast<float>(argv[6]);
        float maxPhil = boost::lexical_cast<float>(argv[7]);
        float step = boost::lexical_cast<float>(argv[8]);
        
        for (float i = minNose; i<=maxNose; i+=step) {
            for (float j = minPhil; j <=maxPhil; j+=step) {
                vector<float> p;
                p.push_back(i);
                p.push_back(j);
                rowVectors.push_back(p);
            }
        }
    }
    

    ASM_Gaze_Tracker poseTracker(fs::path(argv[1])/ "trackermodel.yaml",fs::path(argv[2]));
    
    fs::path testFile(argv[3]);
    vector<vector<float> > rawNumbers = parseTextTableFile(testFile.string()," ");
    vector<vector<Point2f> > featurePoints = extractFeaturePoints(rawNumbers);
    
    for (int x = 0; x<rowVectors.size(); x++) {
        float i = rowVectors[x][0], j = rowVectors[x][1];
        cout<<testFile.string()<<" step:"<<i<<" "<<j<<endl;
        CSVFileWriter csvlogger;
        for (int k = 0; k < rawNumbers.size(); k++) {
            poseTracker.facialPointsIn3D[4].z = i;
            poseTracker.facialPointsIn3D[5].z = i;
            poseTracker.facialPointsIn3D[6].z = j;
            
            poseTracker.isTrackingSuccess = true;
            poseTracker.leftEyePoint = featurePoints[k][0];
            poseTracker.rightEyePoint= featurePoints[k][1];
            poseTracker.tracker.points= vector<Point2f>(featurePoints[k].begin()+2,featurePoints[k].end());
            poseTracker.estimateFacePose();
            poseTracker.estimatePupilCenterFaceCoordinates();
            csvlogger.addSlot(poseTracker.toDataSlot());
        }
        csvlogger.writeToFile(testFile.parent_path() / (testFile.stem().string() + ".test_"+boost::lexical_cast<string>(i)+"_"+boost::lexical_cast<string>(j)));
    }
}