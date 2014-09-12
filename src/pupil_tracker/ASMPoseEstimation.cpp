//
//  ASMPoseEstimation.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-9-11.
//
//

#include "ASMPoseEstimation.h"

ASM_PoseEstimator::ASM_PoseEstimator(const fs::path & trackermodelpath, const fs::path & cameraProfile) {
    
    tracker = load_ft<face_tracker>(trackermodelpath.string());
    tracker.detector.baseDir = trackermodelpath.parent_path().string() + fs::path("/").make_preferred().native();
    
    readCameraProfile(cameraProfile, cameraMatrix, distCoeffs);
}

void ASM_PoseEstimator::projectPoints(InputArray sourcePoints, OutputArray destPoints) {
    cv::projectPoints(sourcePoints, rvec, tvec, cameraMatrix, distCoeffs, destPoints);
}

void ASM_PoseEstimator::processFrame(vector<Point2f> featurePointsInImage) {
    solvePnP(facialPointsIn3D, featurePointsInImage, cameraMatrix, distCoeffs, rvec, tvec);
}

void ASM_PoseEstimator::findBestFrontalFaceShapeIn3D()  {
    int currentIndex = -1;
    vector<vector<Point2f> > pointsSeries = tracker.smodel.matY2pts();
    
    currentIndex = 0;
    vector<Point2f> points = pointsSeries[currentIndex];
    Point2f center = points[0]*0.5f + points[1]*0.5f;
    float normVaue = tracker.annotations.getDistanceBetweenOuterCanthuses()/norm(points[3]-points[2]);
    for (int i = 0 ; i < points.size(); i++) {
        points[i] =points[i]-  center;
        points[i] *=normVaue;
    }
    vector<Point3f> faceFeatures;
    for (int i =0 ; i < points.size() ; i ++) {
        faceFeatures.push_back(Point3f(points[i].x,points[i].y,0));
    }
    faceFeatures[4].z = 8;
    faceFeatures[5].z = 8;
    
    facialPointsIn3D = faceFeatures;
}