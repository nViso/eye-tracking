//
//  ASMPupilTracker.h
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-31.
//
//

#include <iostream>
#include "asm_face/ft.hpp"
#include "jzp_lib/jzplib_all.h"

class ASM_Pupil_Tracker {
public:
    ASM_Pupil_Tracker(fs::path trackermodel);
    void processFrame(const cv::Mat & im);
    vector<float> toDataSlot();
    void reDetectFace();
    Point2f glabellaPoint;
    float eyePairTileAngle;
    Point2f leftEyePoint, rightEyePoint;
    vector<Point2f> canthusPts;
    vector<Point2f> nosePts;
    
    Mat rotated_img;
    Mat rotationMatrix;
    
    fs::path trackerFilePath;
    face_tracker tracker;
    
    
private:
    
    
};
