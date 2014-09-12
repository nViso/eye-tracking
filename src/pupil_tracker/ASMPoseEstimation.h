//
//  ASMPoseEstimation.h
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-9-11.
//
//

#include <iostream>
#include "asm_face/ft.hpp"
#include "jzp_lib/jzplib_all.h"

class ASM_PoseEstimator {
public:
    ASM_PoseEstimator(const fs::path & trackermodel, const fs::path & cameraProfile);
    void projectPoints(InputArray sourcePoints, OutputArray destPoints );
    void processFrame(vector<Point2f> featurePointsInImage);
    Mat rvec;
    Mat tvec;
    Mat cameraMatrix;
    Mat distCoeffs;
    vector<Point3f> facialPointsIn3D;
    face_tracker tracker;
    
private:
    void findBestFrontalFaceShapeIn3D() ;
};